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

		const VkSurfaceKHR& Get() const noexcept { return m_Surface; }

		VulkanSurface(const VulkanSurface&) = delete;
		VulkanSurface& operator=(const VulkanSurface&) = delete;
		VulkanSurface(VulkanSurface&&) = delete;
		VulkanSurface& operator=(VulkanSurface&&) = delete;

	private:
		VkSurfaceKHR m_Surface;
		VkInstance m_VkInstance;
	};
} // End of namespace