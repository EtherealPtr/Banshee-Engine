#include "VulkanRenderContext.h"
#include "Graphics/Window.h"

namespace Banshee
{
	VulkanRenderContext::VulkanRenderContext(const Window& _window) :
		m_Window{ _window },
		m_Instance{},
		m_Surface{ _window.GetWindow(), m_Instance.Get() },
		m_Device{ m_Instance.Get(), m_Surface.Get() },
		m_Swapchain{ m_Device.GetLogicalDevice(), m_Device.GetPhysicalDevice(), m_Surface.Get(), _window.GetWidth(), _window.GetHeight() }
	{}

	void VulkanRenderContext::RecreateSwapchain(const uint32 _w, const uint32 _h)
	{
		m_Swapchain.RecreateSwapchain(_w, _h);
	}
} // End of namespace