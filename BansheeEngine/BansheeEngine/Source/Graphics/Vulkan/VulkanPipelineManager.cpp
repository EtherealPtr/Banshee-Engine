#include "VulkanPipelineManager.h"

namespace Banshee
{
	VulkanPipelineManager::VulkanPipelineManager(const VkDevice& _device, const VkRenderPass& _sceneRenderPass, const VkRenderPass& _shadowRenderPass, const VkDescriptorSetLayout& _sceneDescriptorSetLayout, const VkDescriptorSetLayout& _shadowDescriptorSetLayout, const uint32 _width, const uint32 _height)
	{
		m_Pipelines.emplace(std::piecewise_construct,
			std::forward_as_tuple(SceneDataEnum::Scene),
			std::forward_as_tuple(_device, _sceneRenderPass, _sceneDescriptorSetLayout, _width, _height, "Shaders/Standard/standard_vert.spv", "Shaders/Standard/standard_frag.spv"));

		m_Pipelines.emplace(std::piecewise_construct,
			std::forward_as_tuple(SceneDataEnum::DepthOnly),
			std::forward_as_tuple(_device, _shadowRenderPass, _shadowDescriptorSetLayout, 1024, 1024, "Shaders/Depth/depth.spv"));
	}

	const VulkanGraphicsPipeline* const VulkanPipelineManager::GetPipeline(const SceneDataEnum _type)
	{
		return &m_Pipelines.at(_type);
	}
} // End of namespace