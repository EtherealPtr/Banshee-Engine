#include "VulkanFrameResources.h"
#include <vulkan/vulkan.h>

namespace Banshee
{
	VulkanFrameResources::VulkanFrameResources(const VulkanDevice& _device, const VulkanSwapchain& _swapchain, const VkCommandPool& _commandPool, const VulkanRenderPassManager& _renderPassManager, const VulkanDepthBuffer& _sceneDepthBuffer, const VulkanDepthBuffer& _shadowDepthBuffer, const uint32 _shadowWidth, const uint32 _shadowHeight) :
		m_FramebufferShadow{ _device.GetLogicalDevice(), _renderPassManager.GetRenderPass(RenderPassType::DepthOnly)->Get(), VK_NULL_HANDLE, _shadowDepthBuffer.GetImageView(), _shadowWidth, _shadowHeight },
		m_CommandBuffers{ _device.GetLogicalDevice(), _commandPool, static_cast<uint16>(_swapchain.GetImageViews().size()) },
		m_Semaphore{ _device.GetLogicalDevice(), static_cast<uint16>(_swapchain.GetImageViews().size()) },
		m_InFlightFences{ _device.GetLogicalDevice(), static_cast<uint16>(_swapchain.GetImageViews().size()) }
	{
		const uint32 imageCount{ static_cast<uint32>(_swapchain.GetImageViews().size()) };
		m_Framebuffers.reserve(imageCount);
		auto renderPass{ _renderPassManager.GetRenderPass(RenderPassType::Scene)->Get() };

		for (uint32 i = 0; i < imageCount; ++i)
		{
			m_Framebuffers.emplace_back(_device.GetLogicalDevice(), renderPass, _swapchain.GetImageViews()[i], _sceneDepthBuffer.GetImageView(), _swapchain.GetWidth(), _swapchain.GetHeight());
		}
	}

	const VkCommandBuffer& VulkanFrameResources::BeginFrame(const uint8 _imgIndex) noexcept
	{
		m_InFlightFences.Wait(_imgIndex);
		vkResetCommandBuffer(m_CommandBuffers.Get()[_imgIndex], 0);
		m_CommandBuffers.Begin(_imgIndex);
		return m_CommandBuffers.Get()[_imgIndex];
	}
	
	void VulkanFrameResources::EndFrame(const uint8 _imgIndex) noexcept
	{
		m_CommandBuffers.End(_imgIndex);
	}
} // End of namespace