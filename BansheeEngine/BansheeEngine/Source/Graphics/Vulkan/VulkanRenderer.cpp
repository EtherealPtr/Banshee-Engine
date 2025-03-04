#include "VulkanRenderer.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Entity/EntityManager.h"
#include "Graphics/MeshData.h"
#include "Graphics/Window.h"
#include <array>
#include <vulkan/vulkan_core.h>

namespace Banshee
{
	VulkanRenderer::VulkanRenderer(const Window& _window) :
		m_RenderContext{ _window },
		m_DescriptorPool{ m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetSwapchain().GetImageViews().size() * 2 },
		m_SceneResources{ m_RenderContext, m_DescriptorPool.Get(), 0 },
		m_ShadowSceneResources{ m_RenderContext, m_DescriptorPool.Get(), VK_IMAGE_USAGE_SAMPLED_BIT },
		m_FramebufferShadow{ m_RenderContext.GetDevice().GetLogicalDevice(), m_ShadowSceneResources.GetRenderPass(), VK_NULL_HANDLE, m_ShadowSceneResources.GetDepthBuffer().GetImageView(), m_ShadowSceneResources.GetShadowWidth(), m_ShadowSceneResources.GetShadowHeight() },
		m_CommandPool{ m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetDevice().GetQueueIndices().m_GraphicsQueueFamilyIndex },
		m_CommandBuffers{ m_RenderContext.GetDevice().GetLogicalDevice(), m_CommandPool.Get(), static_cast<uint16>(m_RenderContext.GetSwapchain().GetImageViews().size()) },
		m_Semaphore{ m_RenderContext.GetDevice().GetLogicalDevice(), static_cast<uint16>(m_RenderContext.GetSwapchain().GetImageViews().size()) },
		m_InFlightFences{ m_RenderContext.GetDevice().GetLogicalDevice(), static_cast<uint16>(m_RenderContext.GetSwapchain().GetImageViews().size()) },
		m_TextureSampler{ m_RenderContext.GetDevice() },
		m_Textures{ m_RenderContext.GetDevice(), m_CommandPool.Get() },
		m_VertexBufferManager{ m_RenderContext.GetDevice(), m_CommandPool.Get(), m_RenderContext.GetDevice().GetGraphicsQueue() },
		m_Camera{ 80.0f, static_cast<float>(_window.GetWidth()) / _window.GetHeight(), 0.1f, 100.0f, _window.GetWindow() },
		m_MeshSystem{ m_VertexBufferManager },
		m_CurrentFrameIndex{ 0 }
	{
		InitializeGraphicsComponents();

		m_Framebuffers.reserve(m_RenderContext.GetSwapchain().GetImageViews().size());

		for (size_t i = 0; i < m_RenderContext.GetSwapchain().GetImageViews().size(); ++i)
		{
			m_Framebuffers.emplace_back(m_RenderContext.GetDevice().GetLogicalDevice(), m_SceneResources.GetRenderPass(), m_RenderContext.GetSwapchain().GetImageViews().at(i), m_SceneResources.GetDepthBuffer().GetImageView(), m_RenderContext.GetSwapchain().GetWidth(), m_RenderContext.GetSwapchain().GetHeight());
		}

		m_SceneResources.UpdateMeshUniformBuffers(m_MeshSystem);
		m_Textures.UploadTextures();
		m_SceneResources.SetSceneTextures(m_Textures.GetTextureImageViews(), m_TextureSampler.Get());
		m_SceneResources.SetSceneShadowMap(m_ShadowSceneResources.GetDepthBuffer().GetImageView(), m_TextureSampler.Get());
		m_SceneResources.UpdateStaticDescriptorSets();

		UpdateShadowSceneDescriptorSets();

		BE_LOG(LogCategory::Trace, "[RENDERER]: Vulkan renderer initialized");
	}

	VulkanRenderer::~VulkanRenderer()
	{
		vkDeviceWaitIdle(m_RenderContext.GetDevice().GetLogicalDevice());

		BE_LOG(LogCategory::Trace, "[RENDERER]: Vulkan renderer shutting down");
	}

	void VulkanRenderer::InitializeGraphicsComponents()
	{
		for (const auto& entity : EntityManager::GetAllEntities())
		{
			m_MeshSystem.ProcessComponents(entity);
			m_LightSystem.ProcessComponents(entity);
			m_TransformationSystem.ProcessComponents(entity);
		}
	}

	void VulkanRenderer::UpdateShadowSceneDescriptorSets()
	{
		m_ShadowSceneResources.UpdateShadowUniformBuffer(m_LightSystem);
		glm::mat4 lightSpaceMatrix{ m_ShadowSceneResources.GetLightSpaceMatrix() };
		m_SceneResources.UpdateLightSpaceUniformBuffer(lightSpaceMatrix);
	}

	void VulkanRenderer::DrawFrame(const double _deltaTime)
	{
		uint32 imgIndex{ 0 };

		m_InFlightFences.Wait(m_CurrentFrameIndex);

		const VkResult result{ vkAcquireNextImageKHR
		(
			m_RenderContext.GetDevice().GetLogicalDevice(),
			m_RenderContext.GetSwapchain().Get(),
			UINT64_MAX,
			m_Semaphore.Get().at(m_CurrentFrameIndex).first,
			VK_NULL_HANDLE,
			&imgIndex
		) };

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			vkDeviceWaitIdle(m_RenderContext.GetDevice().GetLogicalDevice());

			const uint32 newWidth{ m_RenderContext.GetWindow().GetWidth() };
			const uint32 newHeight{ m_RenderContext.GetWindow().GetHeight() };

			if (newWidth == 0 || newHeight == 0)
			{
				return;
			}

			m_RenderContext.RecreateSwapchain(newWidth, newHeight);
			m_SceneResources.RecreateDepthBuffer(newWidth, newHeight, 0);
			m_ShadowSceneResources.RecreateDepthBuffer(VK_IMAGE_USAGE_SAMPLED_BIT);
			m_FramebufferShadow.RecreateFramebuffer(m_ShadowSceneResources.GetShadowWidth(), m_ShadowSceneResources.GetShadowHeight(), VK_NULL_HANDLE, m_ShadowSceneResources.GetDepthBuffer().GetImageView());
			
			for (size_t i = 0; i < m_Framebuffers.size(); ++i)
			{
				m_Framebuffers.at(i).RecreateFramebuffer(m_RenderContext.GetSwapchain().GetWidth(), m_RenderContext.GetSwapchain().GetHeight(), m_RenderContext.GetSwapchain().GetImageViews().at(i), m_SceneResources.GetDepthBuffer().GetImageView());
			}

			m_SceneResources.SetSceneShadowMap(m_ShadowSceneResources.GetDepthBuffer().GetImageView(), m_TextureSampler.Get());
			m_SceneResources.UpdateStaticDescriptorSets();

			vkResetCommandPool(m_RenderContext.GetDevice().GetLogicalDevice(), m_CommandPool.Get(), 0);
			return;
		}

		m_InFlightFences.Reset(m_CurrentFrameIndex);

		m_Camera.ProcessInput(_deltaTime);

		VkSemaphore waitSemaphore{ m_Semaphore.Get().at(m_CurrentFrameIndex).first };
		VkSemaphore signalSemaphore{ m_Semaphore.Get().at(m_CurrentFrameIndex).second };

		VkCommandBuffer cmdBuffer{ m_CommandBuffers.Get().at(imgIndex) };

		vkResetCommandBuffer(cmdBuffer, 0);
		RecordRenderCommands(imgIndex);

		// Submit the command buffer and signal the next semaphore
		m_CommandBuffers.Submit
		(
			imgIndex,
			m_RenderContext.GetDevice().GetGraphicsQueue(),
			waitSemaphore,
			signalSemaphore,
			m_InFlightFences.Get().at(m_CurrentFrameIndex),
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		);

		// Present the current image and wait for the current signal semaphore
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &signalSemaphore;

		VkSwapchainKHR swapchains[] = { m_RenderContext.GetSwapchain().Get() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &imgIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(m_RenderContext.GetDevice().GetPresentationQueue(), &presentInfo);
		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_RenderContext.GetSwapchain().GetImageViews().size();
	}

	void VulkanRenderer::RecordRenderCommands(const uint32 _imgIndex)
	{
		const VkCommandBuffer cmdBuffer{ m_CommandBuffers.Get().at(_imgIndex) };
		m_CommandBuffers.Begin(_imgIndex);

		RenderShadowMap(cmdBuffer);

		ViewProjMatrix viewProjMatrix{ m_Camera.GetViewProjMatrix() };
		viewProjMatrix.m_Proj[1][1] *= -1.0f;
		m_SceneResources.UpdateVPUniformBuffers(viewProjMatrix, _imgIndex);

		uint8 lightCount{ 0 };
		auto lightData{ m_LightSystem.UpdateLightData(lightCount) };
		m_SceneResources.UpdateLightUniformBuffer(std::span{ lightData }.first(lightCount), _imgIndex);

		m_SceneResources.UpdateDescriptorSets(_imgIndex);

		RenderScene(cmdBuffer, _imgIndex);

		m_CommandBuffers.End(_imgIndex);
	}

	void VulkanRenderer::RenderShadowMap(const VkCommandBuffer& _cmdBuffer)
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_ShadowSceneResources.GetRenderPass();
		renderPassInfo.framebuffer = m_FramebufferShadow.GetFramebuffer();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VkExtent2D({ m_ShadowSceneResources.GetShadowWidth(), m_ShadowSceneResources.GetShadowHeight() });

		constexpr VkClearDepthStencilValue clearDepthStencil{ 1.0f, 0 };

		std::array<VkClearValue, 1> clearAttachments{};
		clearAttachments.at(0).depthStencil = clearDepthStencil;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = clearAttachments.data();

		vkCmdBeginRenderPass(_cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_ShadowSceneResources.GetShadowWidth());
		viewport.height = static_cast<float>(m_ShadowSceneResources.GetShadowHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(_cmdBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VkExtent2D({ m_ShadowSceneResources.GetShadowWidth(), m_ShadowSceneResources.GetShadowHeight() });
		vkCmdSetScissor(_cmdBuffer, 0, 1, &scissor);

		for (const auto& subMesh : m_MeshSystem.GetAllSubMeshes())
		{
			const glm::mat4 entityModelMatrix{ m_TransformationSystem.GetEntityModelMatrix(subMesh.GetEntityId()) };

			// Bind vertex & index buffers
			const VulkanVertexBuffer* const vertexBuffer{ m_MeshSystem.GetVertexBuffer(subMesh.GetVertexBufferId()) };
			const VkDeviceSize indexOffset{ subMesh.GetIndexOffset() * sizeof(uint32) };
			vertexBuffer->Bind(_cmdBuffer, indexOffset);

			// Bind graphics pipeline
			vkCmdBindPipeline(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_ShadowSceneResources.GetPipeline().Get());

			// Bind descriptor set
			auto currentDescriptorSet{ m_ShadowSceneResources.GetDescriptorSetShadow().Get() };
			vkCmdBindDescriptorSets(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_ShadowSceneResources.GetPipeline().GetLayout(), 0, 1, &currentDescriptorSet, 0, nullptr);

			// Push constants
			const glm::mat4& modelMatrix{ entityModelMatrix * subMesh.GetModelMatrix() };
			vkCmdPushConstants(_cmdBuffer, m_ShadowSceneResources.GetPipeline().GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &modelMatrix);

			vkCmdDrawIndexed(_cmdBuffer, subMesh.GetIndexCount(), 1, 0, 0, 0);
		}

		vkCmdEndRenderPass(_cmdBuffer);
	}

	void VulkanRenderer::RenderScene(const VkCommandBuffer& _cmdBuffer, const uint32 _imgIndex)
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_SceneResources.GetRenderPass();
		renderPassInfo.framebuffer = m_Framebuffers.at(_imgIndex).GetFramebuffer();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VkExtent2D({ m_RenderContext.GetSwapchain().GetWidth(), m_RenderContext.GetSwapchain().GetHeight() });

		// Clear attachments
		constexpr VkClearColorValue clearColor{ 0.1f, 0.1f, 0.1f, 1.0f };
		constexpr VkClearDepthStencilValue clearDepthStencil{ 1.0f, 0 };

		std::array<VkClearValue, 2> clearAttachments{};
		clearAttachments.at(0).color = clearColor;
		clearAttachments.at(1).depthStencil = clearDepthStencil;
		renderPassInfo.clearValueCount = static_cast<uint32>(clearAttachments.size());
		renderPassInfo.pClearValues = clearAttachments.data();

		vkCmdBeginRenderPass(_cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_RenderContext.GetSwapchain().GetWidth());
		viewport.height = static_cast<float>(m_RenderContext.GetSwapchain().GetHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(_cmdBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VkExtent2D({ m_RenderContext.GetSwapchain().GetWidth(), m_RenderContext.GetSwapchain().GetHeight() });
		vkCmdSetScissor(_cmdBuffer, 0, 1, &scissor);

		for (const auto& subMesh : m_MeshSystem.GetAllSubMeshes())
		{
			const glm::mat4 entityModelMatrix{ m_TransformationSystem.GetEntityModelMatrix(subMesh.GetEntityId()) };

			// Bind vertex & index buffers
			const VulkanVertexBuffer* const vertexBuffer{ m_MeshSystem.GetVertexBuffer(subMesh.GetVertexBufferId()) };
			const VkDeviceSize indexOffset{ subMesh.GetIndexOffset() * sizeof(uint32) };
			vertexBuffer->Bind(_cmdBuffer, indexOffset);

			// Bind graphics pipeline
			vkCmdBindPipeline(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_SceneResources.GetPipeline().Get());

			// Bind descriptor set
			const uint32 dynamicOffset{ static_cast<uint32>(m_SceneResources.GetMaterialMemoryAlignment()) * subMesh.GetMeshId() };
			auto currentDescriptorSet{ m_SceneResources.GetDescriptorSets().at(_imgIndex).Get() };
			vkCmdBindDescriptorSets(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_SceneResources.GetPipeline().GetLayout(), 0, 1, &currentDescriptorSet, 1, &dynamicOffset);

			// Push constants
			const glm::mat4& modelMatrix{ entityModelMatrix * subMesh.GetModelMatrix() };
			const PushConstant pc{ modelMatrix, m_Camera.GetPosition(), subMesh.GetTexId() };
			vkCmdPushConstants(_cmdBuffer, m_SceneResources.GetPipeline().GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &pc);

			vkCmdDrawIndexed(_cmdBuffer, subMesh.GetIndexCount(), 1, 0, 0, 0);
		}

		vkCmdEndRenderPass(_cmdBuffer);
	}
} // End of namespace