#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkRenderPass_T* VkRenderPass;
typedef struct VkPipelineLayout_T* VkPipelineLayout;
typedef struct VkPipeline_T* VkPipeline;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;

namespace Banshee
{
	class VulkanGraphicsPipeline
	{
	public:
		VulkanGraphicsPipeline(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const VkDescriptorSetLayout& _descriptorSetLayout, const uint32 _w, const uint32 _h, const char* _vertShaderPath = "Shaders/Standard/standard_vert.spv", const char* _fragShaderPath = "Shaders/Standard/standard_frag.spv");
		~VulkanGraphicsPipeline();

		VkPipeline Get() const noexcept { return m_GraphicsPipeline; }
		VkPipelineLayout GetLayout() const noexcept { return m_PipelineLayout; }

	private:
		VkDevice m_LogicalDevice;
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
	};
} // End of Banshee namespace