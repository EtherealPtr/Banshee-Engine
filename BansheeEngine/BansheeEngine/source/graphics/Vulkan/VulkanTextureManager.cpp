#include "VulkanTextureManager.h"
#include "VulkanUtils.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/ResourceManager/Image/ImageManager.h"
#include "Foundation/ResourceManager/Image/Image.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanTextureManager::VulkanTextureManager(const VkDevice& _device, const VkPhysicalDevice& _gpu, const VkQueue& _graphicsQueue, const VkCommandPool& _commandPool) :
		m_LogicalDevice(_device),
		m_PhysicalDevice(_gpu),
		m_GraphicsQueue(_graphicsQueue),
		m_CommandPool(_commandPool),
		m_TextureImageFormat(VK_FORMAT_R8G8B8A8_SRGB)
	{
		const std::vector<std::shared_ptr<Image>>& images = ResourceManager::Instance().GetImageManager()->GetImages();
		
		for (const auto& image : images)
		{
			CreateStagingBuffer(image->imageSize, image->pixels, image->imageWidth, image->imageHeight);
		}
	}

	VulkanTextureManager::~VulkanTextureManager()
	{
		for (const auto& image : m_TextureImages)
		{
			vkDestroyImageView(m_LogicalDevice, image.imageView, nullptr);
			vkDestroyImage(m_LogicalDevice, image.image, nullptr);
			vkFreeMemory(m_LogicalDevice, image.imageMemory, nullptr);
		}
	}

	std::vector<VkImageView> VulkanTextureManager::GetTextureImageViews() const
	{
		std::vector<VkImageView> imageViews{};
		imageViews.reserve(m_TextureImages.size());

		for (const auto& image : m_TextureImages)
		{
			imageViews.emplace_back(image.imageView);
		}

		return imageViews;
	}

	void VulkanTextureManager::CreateStagingBuffer(const uint64 _sizeOfBuffer, const unsigned char* _pixels, const uint32 _imgW, const uint32 _imgH)
	{
		VkBuffer stagingBuffer{};
		VkDeviceMemory stagingBufferMemory{};
		
		VulkanUtils::CreateBuffer
		(
			m_LogicalDevice,
			m_PhysicalDevice,
			_sizeOfBuffer,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory
		);

		void* data = nullptr;
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, _sizeOfBuffer, 0, &data);
		memcpy(data, _pixels, static_cast<size_t>(_sizeOfBuffer));
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		CreateTextureImage(stagingBuffer, _imgW, _imgH);

		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		stagingBufferMemory = VK_NULL_HANDLE;
		stagingBuffer = VK_NULL_HANDLE;
	}

	void VulkanTextureManager::CreateTextureImage(const VkBuffer& _buffer, const uint32 _imgW, const uint32 _imgH)
	{
		VkImage textureImage{};
		VkImageView textureImageView{};
		VkDeviceMemory textureImageMemory{};

		VulkanUtils::CreateImage
		(
			m_LogicalDevice,
			m_PhysicalDevice,
			_imgW,
			_imgH,
			m_TextureImageFormat,
			VK_IMAGE_TILING_OPTIMAL,
			VkImageUsageFlagBits(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			textureImage,
			textureImageMemory
		);

		// Transition image layout to read pixels from VkBuffer
		VulkanUtils::TransitionImageLayout
		(
			m_LogicalDevice,
			m_CommandPool,
			m_GraphicsQueue,
			textureImage,
			m_TextureImageFormat,
			VK_IMAGE_LAYOUT_UNDEFINED,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL
		);

		// Copy the pixels in VkBuffer to the image
		VulkanUtils::CopyBufferToImage
		(
			m_LogicalDevice,
			m_CommandPool,
			m_GraphicsQueue,
			_buffer,
			textureImage,
			_imgW,
			_imgH
		);

		// Transition image layout so that it can be sampled in the shader
		VulkanUtils::TransitionImageLayout
		(
			m_LogicalDevice,
			m_CommandPool,
			m_GraphicsQueue,
			textureImage,
			m_TextureImageFormat,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		);

		VulkanUtils::CreateImageView(m_LogicalDevice, textureImage, m_TextureImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, textureImageView);
		m_TextureImages.emplace_back(textureImage, textureImageView, textureImageMemory);
		BE_LOG(LogCategory::Info, "[TEXTURE]: Created texture image object (total textures: %d)", m_TextureImages.size());
	}
} // End of Banshee namespace