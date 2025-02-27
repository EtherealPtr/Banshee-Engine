#pragma once

#include "VulkanRenderPass.h"
#include <unordered_map>

namespace Banshee
{
    enum class RenderPassType : uint8
    {
        Scene,
        DepthOnly
    };

    class VulkanRenderPassManager
    {
    public:
        VulkanRenderPassManager(const VkDevice& _device, const VkFormat& _swapchainFormat, const VkFormat& _sceneDepthFormat, const VkFormat& _shadowDepthFormat);

        const VulkanRenderPass* const GetRenderPass(const RenderPassType _type) const;

    private:
        VulkanRenderPass m_RenderPass;
        VulkanRenderPass m_ShadowRenderPass;
        std::unordered_map<RenderPassType, const VulkanRenderPass*> m_RenderPasses;
    };
} // End of namespace