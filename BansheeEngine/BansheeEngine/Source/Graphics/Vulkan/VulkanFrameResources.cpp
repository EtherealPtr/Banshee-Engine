#include "VulkanFrameResources.h"
#include <vulkan/vulkan.h>

namespace Banshee
{
	VulkanFrameResources::VulkanFrameResources(
		const VulkanDevice& _device, 
		const VulkanSwapchain& _swapchain,
		const VkCommandPool& _commandPool,
		const VulkanRenderPassManager& _renderPassManager,
		const VulkanDepthBuffer& _sceneDepthBuffer, 
		const VulkanDepthBuffer& _shadowDepthBuffer) : 
		m_ShadowFramebuffer{ _device.GetLogicalDevice(), _renderPassManager.GetRenderPass(RenderPassType::DepthOnly)->Get(), {_shadowDepthBuffer.GetImageView()}, 2048, 2048 }
	{
		const uint32 imageCount = static_cast<uint32>(_swapchain.GetImageViews().size());

		//m_Framebuffers.reserve(imageCount);

		for (uint32 i = 0; i < imageCount; ++i)
		{
			//auto renderPass{ _renderPassManager.GetRenderPass(RenderPassType::Scene)->Get() };
			//std::vector<VkImageView> attachments = { _swapchain.GetImageViews()[i], _sceneDepthBuffer.GetImageView() };
			//m_Framebuffers.emplace_back(_device.GetLogicalDevice(), renderPass, attachments, _swapchain.GetWidth(), _swapchain.GetHeight());
		}
	}

	//const VkCommandBuffer& VulkanFrameResources::BeginFrame(const uint8 _imgIndex)
	//{
	//	m_InFlightFences.Wait(_imgIndex);
	//	vkResetCommandBuffer(m_CommandBuffers.Get()[_imgIndex], 0);
	//	m_CommandBuffers.Begin(_imgIndex);
	//	return m_CommandBuffers.Get()[_imgIndex];
	//}
	//
	//void VulkanFrameResources::EndFrame(const uint8 _imgIndex)
	//{
	//	m_CommandBuffers.End(_imgIndex);
	//	m_InFlightFences.Reset(_imgIndex);
	//}
} // End of namespace