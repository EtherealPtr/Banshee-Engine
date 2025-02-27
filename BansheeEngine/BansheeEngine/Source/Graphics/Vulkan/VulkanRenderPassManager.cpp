#include "VulkanRenderPassManager.h"

namespace Banshee
{
    VulkanRenderPassManager::VulkanRenderPassManager(const VkDevice& _device, const VkFormat& _swapchainFormat, const VkFormat& _sceneDepthFormat, const VkFormat& _shadowDepthFormat) :
        m_RenderPass{ _device, _swapchainFormat, _sceneDepthFormat },
        m_ShadowRenderPass{ _device, _shadowDepthFormat }
    {
        m_RenderPasses[RenderPassType::Scene] = &m_RenderPass;
        m_RenderPasses[RenderPassType::DepthOnly] = &m_ShadowRenderPass;
    }

    const VulkanRenderPass* const VulkanRenderPassManager::GetRenderPass(const RenderPassType _type) const
    {
        return m_RenderPasses.at(_type);
    }
} // End of namespace