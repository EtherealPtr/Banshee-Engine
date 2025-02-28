#include "VulkanFrameResources.h"
#include <vulkan/vulkan.h>

namespace Banshee
{
	VulkanFrameResources::VulkanFrameResources(const VulkanDevice& _device, const VulkanSwapchain& _swapchain, const VulkanRenderPassManager& _renderPassManager, const VulkanDepthBuffer& _sceneDepthBuffer, const VulkanDepthBuffer& _shadowDepthBuffer, const uint32 _shadowWidth, const uint32 _shadowHeight) :
		m_FramebufferShadow{ _device.GetLogicalDevice(), _renderPassManager.GetRenderPass(SceneDataEnum::DepthOnly)->Get(), VK_NULL_HANDLE, _shadowDepthBuffer.GetImageView(), _shadowWidth, _shadowHeight },
		m_CommandPool{ _device.GetLogicalDevice(), _device.GetQueueIndices().m_GraphicsQueueFamilyIndex },
		m_CommandBuffers{ _device.GetLogicalDevice(), m_CommandPool.Get(), static_cast<uint16>(_swapchain.GetImageViews().size())},
		m_Semaphore{ _device.GetLogicalDevice(), static_cast<uint16>(_swapchain.GetImageViews().size()) },
		m_InFlightFences{ _device.GetLogicalDevice(), static_cast<uint16>(_swapchain.GetImageViews().size()) }
	{
		m_Framebuffers.reserve(_swapchain.GetImageViews().size());
		auto renderPass{ _renderPassManager.GetRenderPass(SceneDataEnum::Scene)->Get() };

		for (size_t i = 0; i < _swapchain.GetImageViews().size(); ++i)
		{
			m_Framebuffers.emplace_back(_device.GetLogicalDevice(), renderPass, _swapchain.GetImageViews().at(i), _sceneDepthBuffer.GetImageView(), _swapchain.GetWidth(), _swapchain.GetHeight());
		}
	}

	const VkCommandBuffer& VulkanFrameResources::BeginFrame(const uint8 _imgIndex) noexcept
	{
		m_InFlightFences.Wait(_imgIndex);
		vkResetCommandBuffer(m_CommandBuffers.Get().at(_imgIndex), 0);
		m_CommandBuffers.Begin(_imgIndex);
		return m_CommandBuffers.Get().at(_imgIndex);
	}
	
	void VulkanFrameResources::EndFrame(const uint8 _imgIndex) noexcept
	{
		m_CommandBuffers.End(_imgIndex);
	}
} // End of namespace