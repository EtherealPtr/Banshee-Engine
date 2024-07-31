#pragma once

#include "Foundation/Platform.h"
#include "Graphics/ShaderType.h"
#include <memory>
#include <unordered_map>

typedef struct VkDevice_T* VkDevice;
typedef struct VkRenderPass_T* VkRenderPass;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;

namespace Banshee
{
    class VulkanGraphicsPipeline;

    class VulkanGraphicsPipelineManager
    {
    public:
        VulkanGraphicsPipelineManager(const VkDevice& _device, const VkRenderPass& _renderPass, const VkDescriptorSetLayout& _descriptorSetLayout, const uint32 _width, const uint32 _height);

        const std::shared_ptr<VulkanGraphicsPipeline>& GetPipeline(const ShaderType _shaderType) { return m_Pipelines[_shaderType]; }

        VulkanGraphicsPipelineManager(const VulkanGraphicsPipelineManager&) = delete;
        VulkanGraphicsPipelineManager& operator=(const VulkanGraphicsPipelineManager&) = delete;
        VulkanGraphicsPipelineManager(VulkanGraphicsPipelineManager&&) = delete;
        VulkanGraphicsPipelineManager& operator=(VulkanGraphicsPipelineManager&&) = delete;

    private:
        std::unordered_map<ShaderType, std::shared_ptr<VulkanGraphicsPipeline>> m_Pipelines;
    };
} // End of Banshee namespace
