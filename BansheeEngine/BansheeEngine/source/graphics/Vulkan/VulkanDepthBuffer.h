#pragma once

#include <cstdint>

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
		VulkanDepthBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint32_t _w, const uint32_t _h);
		~VulkanDepthBuffer();

		VkFormat GetFormat() const { return m_DepthFormat; }
		VkImageView GetImageView() const { return m_DepthImageView; }

	private:
		VkDevice m_LogicalDevice;
		VkImage m_DepthImage;
		VkImageView m_DepthImageView;
		VkDeviceMemory m_DepthImageMemory;
		VkFormat m_DepthFormat;
	};
}
