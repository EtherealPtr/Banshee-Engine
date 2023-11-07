#include "VulkanSwapchain.h"
#include "VulkanUtils.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan.h>
#include <algorithm>
#include <stdexcept>
#include <cassert>

namespace Banshee
{
	VkSurfaceFormatKHR PickSurfaceFormat(const VkPhysicalDevice& _gpu, const VkSurfaceKHR& _surface)
	{
		// Query available surface formats
		uint32 formatCount = 0;
		vkGetPhysicalDeviceSurfaceFormatsKHR(_gpu, _surface, &formatCount, nullptr);
		std::vector<VkSurfaceFormatKHR> surfaceFormats(formatCount);
		vkGetPhysicalDeviceSurfaceFormatsKHR(_gpu, _surface, &formatCount, surfaceFormats.data());

		// Pick surface format
		for (const auto& surfaceFormat : surfaceFormats)
		{
			if (surfaceFormat.format == VK_FORMAT_B8G8R8A8_SRGB && surfaceFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
			{
				BE_LOG(LogCategory::Info, "[SWAPCHAIN]: Selected desired surface format");
				return surfaceFormat;
			}
		}

		BE_LOG(LogCategory::Info, "[SWAPCHAIN]: Selected default surface format");
		return surfaceFormats[0];
	}

	VkPresentModeKHR PickPresentMode(const VkPhysicalDevice& _gpu, const VkSurfaceKHR& _surface)
	{
		// Query available present modes
		uint32 presentModeCount = 0;
		vkGetPhysicalDeviceSurfacePresentModesKHR(_gpu, _surface, &presentModeCount, nullptr);
		std::vector<VkPresentModeKHR> presentModes(presentModeCount);
		vkGetPhysicalDeviceSurfacePresentModesKHR(_gpu, _surface, &presentModeCount, presentModes.data());

		// Pick present mode
		for (const auto& presentMode : presentModes)
		{
			if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
			{
				BE_LOG(LogCategory::Info, "[SWAPCHAIN]: Selected desired presentation mode");
				return presentMode;
			}
		}

		BE_LOG(LogCategory::Info, "[SWAPCHAIN]: Selected default presentation mode");

		// If mailbox present mode is not available, choose FIFO
		return VK_PRESENT_MODE_FIFO_KHR;
	}

	VkExtent2D PickExtent(const VkSurfaceCapabilitiesKHR& _surfaceCapabilities, const uint32 _w, const uint32 _h)
	{
		// If the surface extent is undefined, set it to the window size (_w, _h)
		if (_surfaceCapabilities.currentExtent.width == UINT32_MAX)
		{
			VkExtent2D extent = { _w, _h };
			extent.width = std::max(_surfaceCapabilities.minImageExtent.width, std::min(_surfaceCapabilities.maxImageExtent.width, extent.width));
			extent.height = std::max(_surfaceCapabilities.minImageExtent.height, std::min(_surfaceCapabilities.maxImageExtent.height, extent.height));
			return extent;
		}

		// Otherwise, use the surface extent
		return _surfaceCapabilities.currentExtent;
	}

	uint32 PickImageCount(const VkSurfaceCapabilitiesKHR& _surfaceCapabilities)
	{
		uint32 imageCount = _surfaceCapabilities.minImageCount + 1;

		if (_surfaceCapabilities.maxImageCount > 0 && imageCount > _surfaceCapabilities.maxImageCount)
		{
			BE_LOG(LogCategory::Info, "[SWAPCHAIN]: Picked image count of %d", _surfaceCapabilities.maxImageCount);
			return _surfaceCapabilities.maxImageCount;
		}

		BE_LOG(LogCategory::Info, "[SWAPCHAIN]: Picked image count of %d", imageCount);
		return imageCount;
	}

	VulkanSwapchain::VulkanSwapchain(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const VkSurfaceKHR& _surface, const uint32 _w, const uint32 _h) :
		m_Swapchain(VK_NULL_HANDLE),
		m_Device(_logicalDevice),
		m_SwapchainImages{},
		m_SwapchainImageViews{},
		m_Format(0),
		m_Width(0),
		m_Height(0)
	{
		BE_LOG(LogCategory::Trace, "[SWAPCHAIN]: Creating Vulkan Swapchain");

		// Query surface capabilities
		VkSurfaceCapabilitiesKHR surfaceCapabilities;
		vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_gpu, _surface, &surfaceCapabilities);

		// Pick swapchain surface format and color space
		VkSurfaceFormatKHR surfaceFormat = PickSurfaceFormat(_gpu, _surface);
		m_Format = static_cast<unsigned int>(surfaceFormat.format);

		// Pick swapchain present mode
		VkPresentModeKHR presentMode = PickPresentMode(_gpu, _surface);

		// Pick swapchain extent
		VkExtent2D extent = PickExtent(surfaceCapabilities, _w, _h);
		m_Width = extent.width;
		m_Height = extent.height;

		// Determine number of images in the swapchain
		uint32 imageCount = PickImageCount(surfaceCapabilities);

		// Create swapchain
		VkSwapchainCreateInfoKHR swapchainCreateInfo{};
		swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
		swapchainCreateInfo.surface = _surface;
		swapchainCreateInfo.minImageCount = imageCount;
		swapchainCreateInfo.imageFormat = surfaceFormat.format;
		swapchainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
		swapchainCreateInfo.imageExtent = extent;
		swapchainCreateInfo.imageArrayLayers = 1;
		swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
		swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
		swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
		swapchainCreateInfo.presentMode = presentMode;
		swapchainCreateInfo.clipped = VK_TRUE;

		if (vkCreateSwapchainKHR(_logicalDevice, &swapchainCreateInfo, nullptr, &m_Swapchain) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create Vulkan swapchain\n");
		}

		// Retrieve swapchain images
		vkGetSwapchainImagesKHR(_logicalDevice, m_Swapchain, &imageCount, nullptr);
		m_SwapchainImages.resize(imageCount);
		vkGetSwapchainImagesKHR(_logicalDevice, m_Swapchain, &imageCount, m_SwapchainImages.data());
		assert(m_SwapchainImages.size() > 0);

		// Create image views for the swapchain images
		m_SwapchainImageViews.resize(imageCount);

		for (uint32 i = 0; i < imageCount; ++i)
		{
			m_SwapchainImageViews[i] = VulkanUtils::CreateImageView(_logicalDevice, m_SwapchainImages[i], surfaceFormat.format, VK_IMAGE_ASPECT_COLOR_BIT);
		}

		assert(m_SwapchainImageViews.size() > 0);

		BE_LOG(LogCategory::Info, "[SWAPCHAIN]: Created Vulkan Swapchain");
	}

	VulkanSwapchain::~VulkanSwapchain()
	{
		for (const auto& imageView : m_SwapchainImageViews)
		{
			vkDestroyImageView(m_Device, imageView, nullptr);
		}

		vkDestroySwapchainKHR(m_Device, m_Swapchain, nullptr);
		m_Swapchain = VK_NULL_HANDLE;
	}
}
