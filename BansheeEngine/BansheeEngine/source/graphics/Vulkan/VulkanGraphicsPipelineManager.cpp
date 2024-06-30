#include "VulkanGraphicsPipelineManager.h"
#include "VulkanGraphicsPipeline.h"

namespace Banshee
{
	VulkanGraphicsPipelineManager::VulkanGraphicsPipelineManager(const VkDevice& _device, const VkRenderPass& _renderPass, const VkDescriptorSetLayout& _descriptorSetLayout, const uint32 _width, const uint32 _height)
	{
		m_Pipelines[ShaderType::Standard] = std::make_shared<VulkanGraphicsPipeline>(_device, _renderPass, _descriptorSetLayout, _width, _height);
		m_Pipelines[ShaderType::Unlit] = std::make_shared<VulkanGraphicsPipeline>(_device, _renderPass, _descriptorSetLayout, _width, _height, "Shaders/Unlit/unlit_vert.spv", "Shaders/Unlit/unlit_frag.spv");
	}
} // End of Banshee namespace
