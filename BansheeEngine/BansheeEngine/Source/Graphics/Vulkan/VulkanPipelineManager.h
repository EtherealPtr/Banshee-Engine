#pragma once

#include "VulkanGraphicsPipeline.h"
#include <unordered_map>

namespace Banshee
{
    enum class PipelineType : uint8
    {
        Scene,
        DepthOnly
    };

    class VulkanPipelineManager
    {
    public:
        VulkanPipelineManager(const VkDevice _device, const VkRenderPass _sceneRenderPass, const VkRenderPass _shadowRenderPass, const VkDescriptorSetLayout _sceneDescriptorSetLayout, const VkDescriptorSetLayout _shadowDescriptorSetLayout, const uint32 _width, const uint32 _height);

        const VulkanGraphicsPipeline* const GetPipeline(const PipelineType _type);

    private:
        VulkanGraphicsPipeline m_StandardGraphicsPipeline;
        VulkanGraphicsPipeline m_ShadowPipeline;
        std::unordered_map<PipelineType, const VulkanGraphicsPipeline*> m_Pipelines;
    };

} // End of namespace