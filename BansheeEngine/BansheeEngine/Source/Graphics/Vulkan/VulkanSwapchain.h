#pragma once

#include "Foundation/Platform.h"
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
		VulkanSwapchain(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const VkSurfaceKHR& _surface, const uint32 _w, const uint32 _h);
		~VulkanSwapchain();

		VkSwapchainKHR Get() const noexcept { return m_Swapchain; }
		const std::vector<VkImageView>& GetImageViews() const noexcept { return m_SwapchainImageViews; }
		uint32 GetFormat() const noexcept { return m_Format; }
		uint32 GetWidth() const noexcept { return m_Width; }
		uint32 GetHeight() const noexcept { return m_Height; }

		VulkanSwapchain(const VulkanSwapchain&) = delete;
		VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;
		VulkanSwapchain(VulkanSwapchain&&) = delete;
		VulkanSwapchain& operator=(VulkanSwapchain&&) = delete;

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