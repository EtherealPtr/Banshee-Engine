#include "VulkanSurface.h"
#include "foundation/Logger.h"
#include <vulkan/vulkan.h>
#include <glfw3.h>
#include <stdexcept>

namespace Banshee
{
	VulkanSurface::VulkanSurface(GLFWwindow* _window, const VkInstance& _instance) : 
		m_Surface(VK_NULL_HANDLE),
		m_VkInstance(_instance)
	{
		BE_LOG(LogCategory::trace, "Vulkan surface creation stage: Start");

		if (_window)
		{
			if (glfwCreateWindowSurface(_instance, _window, nullptr, &m_Surface) != VK_SUCCESS)
			{
				BE_LOG(LogCategory::error, "Failed to create a Vulkan surface");
				throw std::runtime_error("ERROR: Failed to create a Vulkan surface");
			}
		}

		BE_LOG(LogCategory::info, "Vulkan surface created successfully");
		BE_LOG(LogCategory::trace, "Vulkan surface creation stage: End");
	}
	
	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(m_VkInstance, m_Surface, nullptr);
		m_Surface = VK_NULL_HANDLE;
		BE_LOG(LogCategory::trace, "Vulkan surface destroyed");
	}
} // End of Banshee namespace