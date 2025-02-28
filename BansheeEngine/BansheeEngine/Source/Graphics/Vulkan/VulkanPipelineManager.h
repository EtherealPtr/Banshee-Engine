#pragma once

#include "VulkanGraphicsPipeline.h"
#include "Graphics/SceneDataEnum.h"
#include <unordered_map>

namespace Banshee
{
    class VulkanPipelineManager
    {
    public:
        VulkanPipelineManager(const VkDevice& _device, const VkRenderPass& _sceneRenderPass, const VkRenderPass& _shadowRenderPass, const VkDescriptorSetLayout& _sceneDescriptorSetLayout, const VkDescriptorSetLayout& _shadowDescriptorSetLayout, const uint32 _width, const uint32 _height);

        const VulkanGraphicsPipeline* const GetPipeline(const SceneDataEnum _type);

        VulkanPipelineManager(const VulkanPipelineManager&) = delete;
        VulkanPipelineManager& operator=(const VulkanPipelineManager&) = delete;
        VulkanPipelineManager(VulkanPipelineManager&&) = delete;
        VulkanPipelineManager& operator=(VulkanPipelineManager&&) = delete;

    private:
        std::unordered_map<SceneDataEnum, VulkanGraphicsPipeline> m_Pipelines;
    };
} // End of namespace