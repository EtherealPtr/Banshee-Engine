#include "VulkanFramebuffer.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <array>

namespace Banshee
{
	VulkanFramebuffer::VulkanFramebuffer(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const std::vector<VkImageView>& _imageViews, const VkImageView& _depthImageView, const uint32 _w, const uint32 _h) :
		m_Device{ _logicalDevice },
		m_RenderPass{ _renderPass },
		m_Framebuffers{ VK_NULL_HANDLE }
	{
		CreateFramebuffers(_w, _h, _imageViews, _depthImageView);
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		CleanUp();
	}
	
	void VulkanFramebuffer::RecreateFramebuffers(const uint32 _w, const uint32 _h, const std::vector<VkImageView>& _imageViews, const VkImageView& _depthImageView)
	{
		CleanUp();
		CreateFramebuffers(_w, _h, _imageViews, _depthImageView);
	}

	void VulkanFramebuffer::CreateFramebuffers(const uint32 _w, const uint32 _h, const std::vector<VkImageView>& _imageViews, const VkImageView& _depthImageView)
	{
		BE_LOG(LogCategory::Trace, "[FRAMEBUFFER]: Creating %d framebuffers", _imageViews.size());
		m_Framebuffers.resize(_imageViews.size(), VK_NULL_HANDLE);

		for (size_t i = 0; i < _imageViews.size(); ++i)
		{
			std::array<VkImageView, 2> imageViews{};
			imageViews[0] = _imageViews[i];
			imageViews[1] = _depthImageView;

			// Create the framebuffer object
			VkFramebufferCreateInfo framebufferCreateInfo{};
			framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
			framebufferCreateInfo.renderPass = m_RenderPass;
			framebufferCreateInfo.attachmentCount = static_cast<uint32>(imageViews.size());
			framebufferCreateInfo.pAttachments = imageViews.data();
			framebufferCreateInfo.width = _w;
			framebufferCreateInfo.height = _h;
			framebufferCreateInfo.layers = 1;

			if (vkCreateFramebuffer(m_Device, &framebufferCreateInfo, nullptr, &m_Framebuffers[i]) != VK_SUCCESS)
			{
				throw std::runtime_error("ERROR: Failed to create a Vulkan framebuffer");
			}
		}

		BE_LOG(LogCategory::Info, "[FRAMEBUFFER]: Created framebuffers");
	}

	void VulkanFramebuffer::CleanUp()
	{
		for (size_t i = 0; i < m_Framebuffers.size(); ++i)
		{
			vkDestroyFramebuffer(m_Device, m_Framebuffers[i], nullptr);
			m_Framebuffers[i] = VK_NULL_HANDLE;
		}

		m_Framebuffers.clear();
	}
} // End of Banshee namespace