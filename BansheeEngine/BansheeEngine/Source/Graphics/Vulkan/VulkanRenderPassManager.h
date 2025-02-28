#pragma once

#include "VulkanRenderPass.h"
#include "Graphics/SceneDataEnum.h"
#include <unordered_map>

namespace Banshee
{
    class VulkanRenderPassManager
    {
    public:
        VulkanRenderPassManager(const VkDevice& _device, const VkFormat _swapchainFormat, const VkFormat _sceneDepthFormat, const VkFormat _shadowDepthFormat);

        const VulkanRenderPass* const GetRenderPass(const SceneDataEnum _type) const;

        VulkanRenderPassManager(const VulkanRenderPassManager&) = delete;
        VulkanRenderPassManager& operator=(const VulkanRenderPassManager&) = delete;
        VulkanRenderPassManager(VulkanRenderPassManager&&) = delete;
        VulkanRenderPassManager& operator=(VulkanRenderPassManager&&) = delete;

    private:
        std::unordered_map<SceneDataEnum, VulkanRenderPass> m_RenderPasses;
    };
} // End of namespace