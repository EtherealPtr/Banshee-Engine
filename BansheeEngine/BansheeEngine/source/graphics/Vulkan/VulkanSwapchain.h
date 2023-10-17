#pragma once

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
		VulkanSwapchain(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const VkSurfaceKHR& _surface, const uint32_t _w, const uint32_t _h);
		~VulkanSwapchain();

		VkSwapchainKHR Get() const { return m_Swapchain; }
		std::vector<VkImageView> GetImageViews() const { return m_SwapchainImageViews; }
		uint32_t GetFormat() const { return m_Format; }
		uint32_t GetWidth() const { return m_Width; }
		uint32_t GetHeight() const { return m_Height; }

	private:
		VkSwapchainKHR m_Swapchain;
		VkDevice m_Device;
		std::vector<VkImage> m_SwapchainImages;
		std::vector<VkImageView> m_SwapchainImageViews;
		uint32_t m_Format;
		uint32_t m_Width;
		uint32_t m_Height;
	};
}
