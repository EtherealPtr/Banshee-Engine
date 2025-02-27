#include "VulkanPipelineManager.h"

namespace Banshee
{
	VulkanPipelineManager::VulkanPipelineManager(const VkDevice _device, const VkRenderPass _sceneRenderPass, const VkRenderPass _shadowRenderPass, const VkDescriptorSetLayout _sceneDescriptorSetLayout, const VkDescriptorSetLayout _shadowDescriptorSetLayout, const uint32 _width, const uint32 _height) :
		m_StandardGraphicsPipeline{ _device, _sceneRenderPass, _sceneDescriptorSetLayout, _width, _height, "Shaders/Standard/standard_vert.spv", "Shaders/Standard/standard_frag.spv" },
		m_ShadowPipeline{ _device, _shadowRenderPass, _shadowDescriptorSetLayout, 2048, 2048, "Shaders/Depth/depth.spv" }
	{
		m_Pipelines[PipelineType::Scene] = &m_StandardGraphicsPipeline;
		m_Pipelines[PipelineType::DepthOnly] = &m_ShadowPipeline;
	}

	const VulkanGraphicsPipeline* const VulkanPipelineManager::GetPipeline(const PipelineType _type)
	{
		return m_Pipelines.at(_type);
	}
} // End of namespace