#pragma once

#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanFramebuffer.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanDepthBuffer.h"
#include "VulkanRenderPassManager.h"
#include <vector>

namespace Banshee
{
    class VulkanFrameResources
    {
    public:
        VulkanFrameResources(const VulkanDevice& _device, const VulkanSwapchain& _swapchain, const VkCommandPool& _commandPool, const VulkanRenderPassManager& _renderPassManager, const VulkanDepthBuffer& _sceneDepthBuffer, const VulkanDepthBuffer& _shadowDepthBuffer, const uint32 _shadowWidth, const uint32 _shadowHeight);

        const VkCommandBuffer& BeginFrame(const uint8 _imgIndex) noexcept;
        void EndFrame(const uint8 _imgIndex) noexcept;

        const std::vector<VulkanFramebuffer>& GetFramebuffers() const noexcept { return m_Framebuffers; }
        const VulkanFramebuffer& GetShadowFramebuffer() const noexcept { return m_FramebufferShadow; }
        VulkanCommandBuffer& GetCommandBuffers() noexcept { return m_CommandBuffers; }
        VulkanSemaphore& GetSemaphore() noexcept { return m_Semaphore; }
        VulkanFence& GetInFlightFences() noexcept { return m_InFlightFences; }

    private:
        std::vector<VulkanFramebuffer> m_Framebuffers;
        VulkanFramebuffer m_FramebufferShadow;
        VulkanCommandBuffer m_CommandBuffers;
        VulkanSemaphore m_Semaphore;
        VulkanFence m_InFlightFences;
    };
} // End of namespace