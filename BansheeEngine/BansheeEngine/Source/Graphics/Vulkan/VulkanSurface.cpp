#include "VulkanSurface.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Banshee
{
	VulkanSurface::VulkanSurface(GLFWwindow* _window, const VkInstance& _instance) :
		m_Surface(VK_NULL_HANDLE),
		m_VkInstance(_instance)
	{
		BE_LOG(LogCategory::Trace, "[SURFACE]: Creating Vulkan surface");

		if (!_window)
		{
			throw std::runtime_error("ERROR: Failed to create a Vulkan surface");
		}

		if (glfwCreateWindowSurface(_instance, _window, nullptr, &m_Surface) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a Vulkan surface");
		}

		BE_LOG(LogCategory::Info, "[SURFACE]: Created Vulkan surface");
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(m_VkInstance, m_Surface, nullptr);
		m_Surface = VK_NULL_HANDLE;
	}
}
