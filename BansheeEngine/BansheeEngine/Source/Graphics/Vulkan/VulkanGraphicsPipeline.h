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
		VulkanGraphicsPipeline(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const VkDescriptorSetLayout& _descriptorSetLayout, const uint32 _w, const uint32 _h, const char* _vertShaderPath, const char* _fragShaderPath);
		VulkanGraphicsPipeline(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const VkDescriptorSetLayout& _descriptorSetLayout, const uint32 _w, const uint32 _h, const char* _vertShaderPath);
		~VulkanGraphicsPipeline();

		VkPipeline Get() const noexcept { return m_GraphicsPipeline; }
		VkPipelineLayout GetLayout() const noexcept { return m_PipelineLayout; }

		VulkanGraphicsPipeline(const VulkanGraphicsPipeline&) = delete;
		VulkanGraphicsPipeline& operator=(const VulkanGraphicsPipeline&) = delete;
		VulkanGraphicsPipeline(VulkanGraphicsPipeline&&) = delete;
		VulkanGraphicsPipeline& operator=(VulkanGraphicsPipeline&&) = delete;

	private:
		VkDevice m_LogicalDevice;
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
	};
} // End of Banshee namespace