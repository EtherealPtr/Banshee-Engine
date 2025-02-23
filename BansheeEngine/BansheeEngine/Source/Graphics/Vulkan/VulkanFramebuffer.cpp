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
		m_Framebuffers{ VK_NULL_HANDLE },
		m_DepthFramebuffer{ VK_NULL_HANDLE }
	{
		CreateFramebuffers(_w, _h, _imageViews, _depthImageView);
	}

	VulkanFramebuffer::VulkanFramebuffer(const VkDevice& _logicalDevice, const VkRenderPass& _renderPass, const VkImageView& _depthImageView, const uint32 _w, const uint32 _h) : 
		m_Device{ _logicalDevice },
		m_RenderPass{ _renderPass },
		m_Framebuffers{ VK_NULL_HANDLE },
		m_DepthFramebuffer{ VK_NULL_HANDLE }
	{
		CreateFramebuffer(_w, _h, _depthImageView);
	}

	VulkanFramebuffer::~VulkanFramebuffer()
	{
		CleanUp();
	}
	
	void VulkanFramebuffer::RecreateFramebuffers(const uint32 _w, const uint32 _h, const std::vector<VkImageView>& _imageViews, const VkImageView& _depthImageView)
	{
		if (m_DepthFramebuffer != VK_NULL_HANDLE)
		{
			CleanUp();
			CreateFramebuffer(_w, _h, _depthImageView);
			return;
		}
		
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
				throw std::runtime_error("ERROR: Failed to create framebuffer");
			}
		}

		BE_LOG(LogCategory::Info, "[FRAMEBUFFER]: Created framebuffers");
	}

	void VulkanFramebuffer::CreateFramebuffer(const uint32 _w, const uint32 _h, const VkImageView& _depthImageView)
	{
		VkFramebufferCreateInfo framebufferCreateInfo{};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.renderPass = m_RenderPass;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = &_depthImageView;
		framebufferCreateInfo.width = _w;
		framebufferCreateInfo.height = _h;
		framebufferCreateInfo.layers = 1;

		if (vkCreateFramebuffer(m_Device, &framebufferCreateInfo, nullptr, &m_DepthFramebuffer) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a depth-only framebuffer");
		}
	}

	void VulkanFramebuffer::CleanUp()
	{
		for (auto framebuffer : m_Framebuffers)
		{
			vkDestroyFramebuffer(m_Device, framebuffer, nullptr);
		}

		m_Framebuffers.clear();

		if (m_DepthFramebuffer != VK_NULL_HANDLE)
		{
			vkDestroyFramebuffer(m_Device, m_DepthFramebuffer, nullptr);
			m_DepthFramebuffer = VK_NULL_HANDLE;
		}
	}
} // End of Banshee namespace