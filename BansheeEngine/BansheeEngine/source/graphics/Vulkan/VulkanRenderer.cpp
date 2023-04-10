#include "VulkanRenderer.h"
#include "../Window.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanRenderer::VulkanRenderer(const Window* _window) :
		m_VkDevice(m_VkInstance.Get(), m_VkSurface.Get()),
		m_VkSurface(_window->GetWindow(), m_VkInstance.Get()),
		m_VkSwapchain(m_VkDevice.GetPhysicalDevice(), m_VkDevice.GetLogicalDevice(), m_VkSurface.Get(), _window->GetWidth(), _window->GetHeight()),
		m_VkRenderPass(m_VkDevice.GetLogicalDevice(), m_VkSwapchain.GetFormat()),
		m_Mesh(PrimitiveShape::triangle),
		m_VertexBuffer(m_VkDevice.GetLogicalDevice(), m_VkDevice.GetPhysicalDevice(), m_Mesh.GetVertices().data(), m_Mesh.GetVertexSizeInBytes()),
		m_VkGraphicsPipeline(m_VkDevice.GetLogicalDevice(), m_VkRenderPass.Get(), m_VkSwapchain.GetWidth(), m_VkSwapchain.GetHeight()),
		m_VkCommandPool(m_VkDevice.GetLogicalDevice(), m_VkDevice.GetQueueIndices().m_GraphicsQueueFamilyIndex),
		m_VkSemaphores(m_VkDevice.GetLogicalDevice(), static_cast<uint8>(m_VkSwapchain.GetImageViews().size())),
		m_VkInFlightFences(m_VkDevice.GetLogicalDevice(), static_cast<uint8>(m_VkSwapchain.GetImageViews().size()))
	{
		CreateFramebuffers();
		CreateCommandBuffers();
		m_CurrentFrameIndex = 0;
	}

	VulkanRenderer::~VulkanRenderer()
	{
		vkDeviceWaitIdle(m_VkDevice.GetLogicalDevice());
	}

	void VulkanRenderer::DrawFrame()
	{
		uint32 imgIndex = 0;

		m_VkInFlightFences.Wait(m_CurrentFrameIndex);
		m_VkInFlightFences.Reset(m_CurrentFrameIndex);

		vkAcquireNextImageKHR(m_VkDevice.GetLogicalDevice(), m_VkSwapchain.Get(), UINT64_MAX, 
							  m_VkSemaphores.Get()[m_CurrentFrameIndex].first, VK_NULL_HANDLE, &imgIndex);

		// Get the semaphores to use for this frame
		VkSemaphore waitSemaphore = m_VkSemaphores.Get()[m_CurrentFrameIndex].first;
		VkSemaphore signalSemaphore = m_VkSemaphores.Get()[m_CurrentFrameIndex].second;

		// Get the command buffer for this image index
		VkCommandBuffer cmdBuffer = m_VkCommandBuffers[imgIndex].Get();

		// Reset the command buffer and record render commands
		vkResetCommandBuffer(cmdBuffer, 0);
		RecordRenderCommands(imgIndex);

		// Submit the command buffer and signal the next semaphore
		m_VkCommandBuffers[imgIndex].Submit(m_VkDevice.GetGraphicsQueue(), waitSemaphore, signalSemaphore, m_VkInFlightFences.Get()[m_CurrentFrameIndex], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

		// Present the current image and wait for the current signal semaphore
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &signalSemaphore;
		
		VkSwapchainKHR swapchains[] = { m_VkSwapchain.Get() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &imgIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(m_VkDevice.GetPresentationQueue(), &presentInfo);

		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_VkSwapchain.GetImageViews().size();
	}
	
	void VulkanRenderer::CreateFramebuffers()
	{
		const size_t numOfSwapImages = m_VkSwapchain.GetImageViews().size();
		m_VkFramebuffers.reserve(numOfSwapImages);

		for (uint8 i = 0; i < numOfSwapImages; ++i)
		{
			m_VkFramebuffers.emplace_back(m_VkDevice.GetLogicalDevice(),
										  m_VkRenderPass.Get(),
										  m_VkSwapchain.GetImageViews()[i],
										  m_VkSwapchain.GetWidth(),
										  m_VkSwapchain.GetHeight());
		}
	}

	void VulkanRenderer::CreateCommandBuffers()
	{
		const size_t numOfSwapImages = m_VkSwapchain.GetImageViews().size();
		m_VkCommandBuffers.reserve(numOfSwapImages);

		for (size_t i = 0; i < numOfSwapImages; ++i)
		{
			m_VkCommandBuffers.emplace_back(m_VkDevice.GetLogicalDevice(), m_VkCommandPool.Get());
		}
	}
	
	void VulkanRenderer::RecordRenderCommands(const uint8 _imgIndex)
	{
		const VkCommandBuffer cmdBuffer = m_VkCommandBuffers[_imgIndex].Get();
		m_VkCommandBuffers[_imgIndex].Begin();
		
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_VkRenderPass.Get();
		renderPassInfo.framebuffer = m_VkFramebuffers[_imgIndex].Get();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VkExtent2D({ m_VkSwapchain.GetWidth(), m_VkSwapchain.GetHeight() });

		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VkGraphicsPipeline.Get());

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_VkSwapchain.GetWidth());
		viewport.height = static_cast<float>(m_VkSwapchain.GetHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VkExtent2D({ m_VkSwapchain.GetWidth(), m_VkSwapchain.GetHeight() });
		vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

		m_VertexBuffer.Bind(cmdBuffer);
		vkCmdDraw(cmdBuffer, 3, 1, 0, 0);
		vkCmdEndRenderPass(cmdBuffer);

		m_VkCommandBuffers[_imgIndex].End();
	}
} // End of Banshee namespace