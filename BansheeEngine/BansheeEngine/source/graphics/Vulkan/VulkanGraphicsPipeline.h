#pragma once

#include "foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkRenderPass_T* VkRenderPass;
typedef struct VkPipelineLayout_T* VkPipelineLayout;
typedef struct VkPipeline_T* VkPipeline;

namespace Banshee
{
	class VulkanGraphicsPipeline
	{
	public:
		VulkanGraphicsPipeline(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const uint32 _w, const uint32 _h);
		~VulkanGraphicsPipeline();

		VkPipeline Get() const { return m_GraphicsPipeline; }

	private:
		VkDevice m_LogicalDevice;
		VkPipelineLayout m_PipelineLayout;
		VkPipeline m_GraphicsPipeline;
	};
} // End of Banshee namespace
