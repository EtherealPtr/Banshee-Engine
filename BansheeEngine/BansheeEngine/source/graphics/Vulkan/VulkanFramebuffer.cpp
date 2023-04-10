#include "VulkanFramebuffer.h"
#include "foundation/Logger.h"
#include <vulkan/vulkan.h>

namespace Banshee
{
	VulkanFramebuffer::VulkanFramebuffer(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const VkImageView& _imageView, const uint32 _w, const uint32 _h) : 
		m_Framebuffer(VK_NULL_HANDLE),
		m_Device(_logicalDevice)
	{
		// Create the framebuffer object
		VkFramebufferCreateInfo framebufferCreateInfo{};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = _renderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = &_imageView;
		framebufferCreateInfo.width = _w;
		framebufferCreateInfo.height = _h;
		framebufferCreateInfo.layers = 1;
		
		if (vkCreateFramebuffer(_logicalDevice, &framebufferCreateInfo, nullptr, &m_Framebuffer) != VK_SUCCESS)
		{
			BE_LOG(LogCategory::warning, "Failed to create a Vulkan framebuffer");
		}

		BE_LOG(LogCategory::info, "Vulkan framebuffer created successfully");
	}
	
	VulkanFramebuffer::~VulkanFramebuffer()
	{
		vkDestroyFramebuffer(m_Device, m_Framebuffer, nullptr);
		m_Framebuffer = VK_NULL_HANDLE;
		BE_LOG(LogCategory::trace, "Vulkan framebuffer destroyed");
	}
} // End of Banshee namespace