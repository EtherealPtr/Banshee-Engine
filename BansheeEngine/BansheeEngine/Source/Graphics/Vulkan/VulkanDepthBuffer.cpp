#include "VulkanDepthBuffer.h"
#include "VulkanUtils.h"
#include <vulkan/vulkan.h>

namespace Banshee
{
	VulkanDepthBuffer::VulkanDepthBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint32 _w, const uint32 _h) :
		m_Device{ _logicalDevice },
		m_GPU{ _gpu },
		m_DepthImage{ VK_NULL_HANDLE },
		m_DepthImageView{ VK_NULL_HANDLE },
		m_DepthImageMemory{ VK_NULL_HANDLE },
		m_DepthFormat{ VK_FORMAT_UNDEFINED }
	{
		CreateDepthBuffer(_w, _h);
	}

	VulkanDepthBuffer::~VulkanDepthBuffer()
	{
		CleanUp();
	}

	void VulkanDepthBuffer::RecreateDepthBuffer(const uint32 _w, const uint32 _h)
	{
		CleanUp();
		CreateDepthBuffer(_w, _h);
	}

	void VulkanDepthBuffer::CreateDepthBuffer(const uint32 _w, const uint32 _h)
	{
		m_DepthFormat = VulkanUtils::FindSupportedFormat
		(
			m_GPU,
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);

		VulkanUtils::CreateImage(m_Device, m_GPU, _w, _h, m_DepthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_DepthImage, m_DepthImageMemory);
		VulkanUtils::CreateImageView(m_Device, m_DepthImage, m_DepthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, m_DepthImageView);
	}

	void VulkanDepthBuffer::CleanUp() const noexcept
	{
		vkDestroyImageView(m_Device, m_DepthImageView, nullptr);
		vkDestroyImage(m_Device, m_DepthImage, nullptr);
		vkFreeMemory(m_Device, m_DepthImageMemory, nullptr);
	}
} // End of Banshee namespace