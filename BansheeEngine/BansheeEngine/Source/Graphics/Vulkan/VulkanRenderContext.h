#pragma once

#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"

namespace Banshee
{
    class Window;

	class VulkanRenderContext
	{
    public:
        VulkanRenderContext(const Window& window);

        void RecreateSwapchain(const uint32 _w, const uint32 _h);
        const VulkanDevice& GetDevice() const noexcept { return m_Device; }
        const VulkanSwapchain& GetSwapchain() const noexcept { return m_Swapchain; }

    private:
        VulkanInstance m_Instance;
        VulkanSurface m_Surface;
        VulkanDevice m_Device;
        VulkanSwapchain m_Swapchain;
	};
} // End of namespace