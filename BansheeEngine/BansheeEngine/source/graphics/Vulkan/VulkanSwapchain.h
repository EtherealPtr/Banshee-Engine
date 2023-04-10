#pragma once

#include "foundation/Platform.h"
#include <vector>

typedef struct VkSwapchainKHR_T* VkSwapchainKHR;
typedef struct VkSurfaceKHR_T* VkSurfaceKHR;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkDevice_T* VkDevice;
typedef struct VkImage_T* VkImage;
typedef struct VkImageView_T* VkImageView;

namespace Banshee
{
	class VulkanSwapchain
	{
	public:
		VulkanSwapchain(const VkPhysicalDevice& _gpu, const VkDevice& _logicalDevice, const VkSurfaceKHR& _surface, const uint32 _w, const uint32 _h);
		~VulkanSwapchain();

		VkSwapchainKHR Get() const { return m_Swapchain; }
		std::vector<VkImageView> GetImageViews() const { return m_SwapchainImageViews; }
		uint32 GetFormat() const { return m_Format; }
		uint32 GetWidth() const { return m_Width; }
		uint32 GetHeight() const { return m_Height; }

	private:
		VkSwapchainKHR m_Swapchain;
		VkDevice m_Device;
		std::vector<VkImage> m_SwapchainImages;
		std::vector<VkImageView> m_SwapchainImageViews;
		uint32 m_Format;
		uint32 m_Width;
		uint32 m_Height;
	};
} // End of Banshee namespace
