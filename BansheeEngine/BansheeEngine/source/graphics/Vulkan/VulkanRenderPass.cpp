#include "VulkanRenderPass.h"
#include "foundation/Logger.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanRenderPass::VulkanRenderPass(const VkDevice& _device, const uint32 _colorFormat) : 
		m_Device(_device),
		m_RenderPass(VK_NULL_HANDLE)
	{
		BE_LOG(LogCategory::trace, "Vulkan render pass creation stage: Start");

		// Define a color attachment for the color output
		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = static_cast<VkFormat>(_colorFormat);
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		// Reference to our color attachment
		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		// Define the subpass
		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		// Define the subpass dependency
		VkSubpassDependency externalToSubpass0Dependency{};
		externalToSubpass0Dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		externalToSubpass0Dependency.dstSubpass = 0;
		externalToSubpass0Dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		externalToSubpass0Dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		externalToSubpass0Dependency.srcAccessMask = 0;
		externalToSubpass0Dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		// Define the render pass
		VkRenderPassCreateInfo renderPassCreateInfo{};
		renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassCreateInfo.attachmentCount = 1;
		renderPassCreateInfo.pAttachments = &colorAttachment;
		renderPassCreateInfo.subpassCount = 1;
		renderPassCreateInfo.pSubpasses = &subpass;
		renderPassCreateInfo.dependencyCount = 1;
		renderPassCreateInfo.pDependencies = &externalToSubpass0Dependency;

		if (vkCreateRenderPass(m_Device, &renderPassCreateInfo, nullptr, &m_RenderPass) != VK_SUCCESS)
		{
			BE_LOG(LogCategory::error, "Failed to create a render pass");
			throw std::runtime_error("ERROR: Failed to create a render pass");
		}

		BE_LOG(LogCategory::info, "Vulkan render pass created successfully");
		BE_LOG(LogCategory::trace, "Vulkan render pass creation stage: End");
	}
	
	VulkanRenderPass::~VulkanRenderPass()
	{
		vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
		m_RenderPass = VK_NULL_HANDLE;
		BE_LOG(LogCategory::trace, "Vulkan render pass destroyed");
	}
} // End of Banshee namespace