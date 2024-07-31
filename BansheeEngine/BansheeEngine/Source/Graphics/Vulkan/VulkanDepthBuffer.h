#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkImage_T* VkImage;
typedef struct VkDeviceMemory_T* VkDeviceMemory;
typedef struct VkImageView_T* VkImageView;
typedef enum VkFormat VkFormat;

namespace Banshee
{
	class VulkanDepthBuffer
	{
	public:
		VulkanDepthBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint32 _w, const uint32 _h);
		~VulkanDepthBuffer();

		VkFormat GetFormat() const noexcept { return m_DepthFormat; }
		VkImageView GetImageView() const noexcept { return m_DepthImageView; }

		VulkanDepthBuffer(const VulkanDepthBuffer&) = delete;
		VulkanDepthBuffer& operator=(const VulkanDepthBuffer&) = delete;
		VulkanDepthBuffer(VulkanDepthBuffer&&) = delete;
		VulkanDepthBuffer& operator=(VulkanDepthBuffer&&) = delete;

	private:
		VkDevice m_LogicalDevice;
		VkImage m_DepthImage;
		VkImageView m_DepthImageView;
		VkDeviceMemory m_DepthImageMemory;
		VkFormat m_DepthFormat;
	};
} // End of Banshee namespace