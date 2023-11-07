#pragma once

typedef struct VkInstance_T* VkInstance;
typedef struct VkSurfaceKHR_T* VkSurfaceKHR;
struct GLFWwindow;

namespace Banshee
{
	class VulkanSurface
	{
	public:
		VulkanSurface(GLFWwindow* _window, const VkInstance& _instance);
		~VulkanSurface();

		VkSurfaceKHR Get() const { return m_Surface; }

	private:
		VkSurfaceKHR m_Surface;
		VkInstance m_VkInstance;
	};
} // End of Banshee namespace