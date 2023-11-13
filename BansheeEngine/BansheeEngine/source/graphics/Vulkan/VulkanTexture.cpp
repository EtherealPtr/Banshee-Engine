#include "VulkanTexture.h"
#include "VulkanUtils.h"
#include "Foundation/File/FileManager.h"
#include "Foundation/Logging/Logger.h"
#include <stb_image.h>
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanTexture::VulkanTexture(const VkDevice& _device, const VkPhysicalDevice& _gpu, const VkQueue& _graphicsQueue, const VkCommandPool& _commandPool, const char* _texturePath) :
		m_LogicalDevice(_device),
		m_PhysicalDevice(_gpu),
		m_GraphicsQueue(_graphicsQueue),
		m_CommandPool(_commandPool),
		m_TextureWidth(0),
		m_TextureHeight(0),
		m_TextureImage(VK_NULL_HANDLE),
		m_TextureImageView(VK_NULL_HANDLE),
		m_TextureImageFormat(VK_FORMAT_R8G8B8A8_SRGB),
		m_TextureImageMemory(VK_NULL_HANDLE)
	{
		BE_LOG(LogCategory::Trace, "[TEXTURE]: Loading texture %s", _texturePath);

		int32 textureChannels = 0;
		auto fullTexturePath = FileManager::Instance().GetEngineResDirPath() + _texturePath;
		stbi_uc* pixels = stbi_load(fullTexturePath.c_str(), &m_TextureWidth, &m_TextureHeight, &textureChannels, STBI_rgb_alpha);
		VkDeviceSize imageSize = m_TextureWidth * m_TextureHeight * 4;

		if (!pixels)
		{
			throw std::runtime_error("ERROR: Failed to load texture image");
		}

		BE_LOG(LogCategory::Info, "[TEXTURE]: Successfully loaded %s", _texturePath);

		CreateStagingBuffer(imageSize, pixels);
		stbi_image_free(pixels);
	}

	VulkanTexture::~VulkanTexture()
	{
		vkDestroyImageView(m_LogicalDevice, m_TextureImageView, nullptr);
		vkDestroyImage(m_LogicalDevice, m_TextureImage, nullptr);
		vkFreeMemory(m_LogicalDevice, m_TextureImageMemory, nullptr);
	}

	void VulkanTexture::CreateStagingBuffer(const uint64 _size, unsigned char* _pixels)
	{
		VkBuffer stagingBuffer{};
		VkDeviceMemory stagingBufferMemory{};
		VulkanUtils::CreateBuffer
		(
			m_LogicalDevice,
			m_PhysicalDevice,
			_size,
			VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			stagingBuffer,
			stagingBufferMemory
		);

		void* data = nullptr;
		vkMapMemory(m_LogicalDevice, stagingBufferMemory, 0, _size, 0, &data);
		memcpy(data, _pixels, static_cast<size_t>(_size));
		vkUnmapMemory(m_LogicalDevice, stagingBufferMemory);

		CreateTextureImage(stagingBuffer);

		vkFreeMemory(m_LogicalDevice, stagingBufferMemory, nullptr);
		vkDestroyBuffer(m_LogicalDevice, stagingBuffer, nullptr);
		stagingBufferMemory = VK_NULL_HANDLE;
		stagingBuffer = VK_NULL_HANDLE;
	}

	void VulkanTexture::CreateTextureImage(const VkBuffer& _buffer)
	{
		BE_LOG(LogCategory::Trace, "[TEXTURE]: Creating texture image object");

		VulkanUtils::CreateImage
		(
			m_LogicalDevice,
			m_PhysicalDevice,
			m_TextureWidth,
			m_TextureHeight,
			m_TextureImageFormat,
			VK_IMAGE_TILING_OPTIMAL,
			VkImageUsageFlagBits(VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT),
			VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			m_TextureImage,
			m_TextureImageMemory
		);

		// Transition image layout to read pixels from VkBuffer
		VulkanUtils::TransitionImageLayout
		(
			m_LogicalDevice,
			m_CommandPool,
			m_GraphicsQueue,
			m_TextureImage,
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
			m_TextureImage,
			m_TextureWidth,

			m_TextureHeight
		);

		// Transition image layout so that it can be sampled in the shader
		VulkanUtils::TransitionImageLayout
		(
			m_LogicalDevice,
			m_CommandPool,
			m_GraphicsQueue,
			m_TextureImage,
			m_TextureImageFormat,
			VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
			VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
		);

		BE_LOG(LogCategory::Info, "[TEXTURE]: Created texture image object");
		m_TextureImageView = VulkanUtils::CreateImageView(m_LogicalDevice, m_TextureImage, m_TextureImageFormat, VK_IMAGE_ASPECT_COLOR_BIT);
	}
} // End of Banshee namespace