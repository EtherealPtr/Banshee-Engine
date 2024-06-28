#include "VulkanGraphicsPipelineManager.h"
#include "VulkanGraphicsPipeline.h"

namespace Banshee
{
	VulkanGraphicsPipelineManager::VulkanGraphicsPipelineManager(const VkDevice& _device, const VkRenderPass& _renderPass, const VkDescriptorSetLayout& _descriptorSetLayout, const uint32 _width, const uint32 _height) :
		m_Device(_device),
		m_RenderPass(_renderPass),
		m_DescriptorSetLayout(_descriptorSetLayout),
		m_Width(_width),
		m_Height(_height)
	{
		m_Pipelines[ShaderType::Standard] = std::make_shared<VulkanGraphicsPipeline>(m_Device, m_RenderPass, m_DescriptorSetLayout, m_Width, m_Height);
		m_Pipelines[ShaderType::Unlit] = std::make_shared<VulkanGraphicsPipeline>(m_Device, m_RenderPass, m_DescriptorSetLayout, m_Width, m_Height, "Shaders/Unlit/unlit_vert.spv", "Shaders/Unlit/unlit_frag.spv");
	}
} // End of Banshee namespace
