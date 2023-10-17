#include "VulkanSurface.h"
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
#include <stdexcept>

namespace Banshee
{
	VulkanSurface::VulkanSurface(GLFWwindow* _window, const VkInstance& _instance) :
		m_Surface(VK_NULL_HANDLE),
		m_VkInstance(_instance)
	{
		if (_window)
		{
			if (glfwCreateWindowSurface(_instance, _window, nullptr, &m_Surface) != VK_SUCCESS)
			{
				throw std::runtime_error("ERROR: Failed to create a Vulkan surface");
			}
		}
	}

	VulkanSurface::~VulkanSurface()
	{
		vkDestroySurfaceKHR(m_VkInstance, m_Surface, nullptr);
		m_Surface = VK_NULL_HANDLE;
	}
}
