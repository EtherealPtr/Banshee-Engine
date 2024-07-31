#include "VulkanDepthBuffer.h"
#include "VulkanUtils.h"
#include <vulkan/vulkan.h>

namespace Banshee
{
	VulkanDepthBuffer::VulkanDepthBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint32 _w, const uint32 _h) :
		m_LogicalDevice{ _logicalDevice },
		m_DepthImage{ VK_NULL_HANDLE },
		m_DepthImageView{ VK_NULL_HANDLE },
		m_DepthImageMemory{ VK_NULL_HANDLE },
		m_DepthFormat{ VK_FORMAT_UNDEFINED }
	{
		m_DepthFormat = VulkanUtils::FindSupportedFormat
		(
			_gpu,
			{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
			VK_IMAGE_TILING_OPTIMAL,
			VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
		);

		VulkanUtils::CreateImage(_logicalDevice, _gpu, _w, _h, m_DepthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, m_DepthImage, m_DepthImageMemory);
		VulkanUtils::CreateImageView(_logicalDevice, m_DepthImage, m_DepthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, m_DepthImageView);
	}

	VulkanDepthBuffer::~VulkanDepthBuffer()
	{
		vkDestroyImageView(m_LogicalDevice, m_DepthImageView, nullptr);
		vkDestroyImage(m_LogicalDevice, m_DepthImage, nullptr);
		vkFreeMemory(m_LogicalDevice, m_DepthImageMemory, nullptr);
	}
} // End of Banshee namespace