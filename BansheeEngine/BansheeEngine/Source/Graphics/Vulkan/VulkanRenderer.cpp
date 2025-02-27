#include "VulkanRenderer.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Entity/EntityManager.h"
#include "Graphics/Components/Light/LightComponent.h"
#include "Graphics/MeshData.h"
#include "Graphics/Window.h"
#include <array>
#include <vulkan/vulkan.h>
#include <Graphics/Components/Light/DirectionalLightComponent.h>

namespace Banshee
{
	constexpr static uint64 g_MaxEntities{ 512 };
	constexpr static uint64 g_ShadowWidth{ 1024 };
	constexpr static uint64 g_ShadowHeight{ 1024 };

	VulkanRenderer::VulkanRenderer(const Window& _window) :
		m_Window(_window),
		m_RenderContext{ _window },
		m_DepthBufferScene{ m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetDevice().GetPhysicalDevice(), m_RenderContext.GetSwapchain().GetWidth(), m_RenderContext.GetSwapchain().GetHeight(), 0 },
		m_DepthBufferShadow{ m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetDevice().GetPhysicalDevice(), g_ShadowWidth, g_ShadowHeight, VK_IMAGE_USAGE_SAMPLED_BIT },
		m_RenderPassManager{ m_RenderContext.GetDevice().GetLogicalDevice(), static_cast<VkFormat>(m_RenderContext.GetSwapchain().GetFormat()), m_DepthBufferScene.GetFormat(), m_DepthBufferShadow.GetFormat() },
		m_CommandPool{ m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetDevice().GetQueueIndices().m_GraphicsQueueFamilyIndex },
		m_TextureSampler{ m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetDevice().GetPhysicalDevice() },
		m_TextureManager{ m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetDevice().GetPhysicalDevice(), m_RenderContext.GetDevice().GetGraphicsQueue(), m_CommandPool.Get() },
		m_DescriptorSetLayoutScene{ m_RenderContext.GetDevice().GetLogicalDevice(), DescriptorSetLayoutType::Standard },
		m_DescriptorSetLayoutShadow{ m_RenderContext.GetDevice().GetLogicalDevice(), DescriptorSetLayoutType::DepthOnly },
		m_PipelineManager{ m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderPassManager.GetRenderPass(RenderPassType::Scene)->Get(), m_RenderPassManager.GetRenderPass(RenderPassType::DepthOnly)->Get(), m_DescriptorSetLayoutScene.Get(), m_DescriptorSetLayoutShadow.Get(), m_RenderContext.GetSwapchain().GetWidth(), m_RenderContext.GetSwapchain().GetHeight() },
		m_FrameResources{ m_RenderContext.GetDevice(), m_RenderContext.GetSwapchain(), m_CommandPool.Get(), m_RenderPassManager, m_DepthBufferScene, m_DepthBufferShadow, g_ShadowWidth, g_ShadowHeight },
		m_DescriptorPool{ m_RenderContext.GetDevice().GetLogicalDevice(), static_cast<uint16>(m_RenderContext.GetSwapchain().GetImageViews().size() * 2) },
		m_DescriptorSetShadow{ m_RenderContext.GetDevice().GetLogicalDevice(), m_DescriptorPool.Get(), m_DescriptorSetLayoutShadow.Get() },
		m_Camera{ 80.0f, static_cast<float>(_window.GetWidth()) / _window.GetHeight(), 0.1f, 100.0f, _window.GetWindow() },
		m_MeshSystem{ m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetDevice().GetPhysicalDevice(), m_CommandPool.Get(), m_RenderContext.GetDevice().GetGraphicsQueue() },
		m_LightSystem{},
		m_CurrentFrameIndex{ 0 },
		m_MaterialDynamicBufferMemAlignment{ 0 },
		m_MaterialDynamicBufferMemBlock{ nullptr, [](Material* _ptr) noexcept { _aligned_free(_ptr); } },
		m_ShadowUniformBuffer(m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetDevice().GetPhysicalDevice(), sizeof(glm::mat4), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT)
	{
		InitializeGraphicsComponents();
		AllocateDynamicBufferSpace();
		CreateDescriptorSetWriteBufferProperties();

		const size_t numOfSwapImages{ m_RenderContext.GetSwapchain().GetImageViews().size() };
		m_VPUniformBuffers.reserve(numOfSwapImages);
		m_MaterialUniformBuffers.reserve(numOfSwapImages);
		m_LightUniformBuffers.reserve(numOfSwapImages);
		m_DescriptorSets.reserve(numOfSwapImages);

		for (size_t i = 0; i < numOfSwapImages; ++i)
		{
			m_VPUniformBuffers.emplace_back(m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetDevice().GetPhysicalDevice(), sizeof(ViewProjMatrix), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			m_MaterialUniformBuffers.emplace_back(m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetDevice().GetPhysicalDevice(), m_MaterialDynamicBufferMemAlignment * g_MaxEntities, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			m_LightUniformBuffers.emplace_back(m_RenderContext.GetDevice().GetLogicalDevice(), m_RenderContext.GetDevice().GetPhysicalDevice(), sizeof(LightBuffer), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
			m_DescriptorSets.emplace_back(m_RenderContext.GetDevice().GetLogicalDevice(), m_DescriptorPool.Get(), m_DescriptorSetLayoutScene.Get());
		}

		UpdateMaterialData();
		m_TextureManager.UploadTextures();
		StaticUpdateDescriptorSets();
		UpdateShadowSceneDescriptorSets();
		BE_LOG(LogCategory::Trace, "[RENDERER]: Vulkan initialized");
	}

	VulkanRenderer::~VulkanRenderer()
	{
		vkDeviceWaitIdle(m_RenderContext.GetDevice().GetLogicalDevice());
		BE_LOG(LogCategory::Trace, "[RENDERER]: Vulkan shutting down");
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

	void VulkanRenderer::AllocateDynamicBufferSpace() noexcept
	{
		const VkDeviceSize minUniformBufferOffset{ m_RenderContext.GetDevice().GetLimits().minUniformBufferOffsetAlignment };
		m_MaterialDynamicBufferMemAlignment = (sizeof(Material) + minUniformBufferOffset - 1) & ~(m_RenderContext.GetDevice().GetLimits().minUniformBufferOffsetAlignment - 1);
		m_MaterialDynamicBufferMemBlock.reset(static_cast<Material*>(_aligned_malloc(m_MaterialDynamicBufferMemAlignment * g_MaxEntities, m_MaterialDynamicBufferMemAlignment)));
	}

	void VulkanRenderer::CreateDescriptorSetWriteBufferProperties()
	{
		constexpr uint32 descriptorWriteBufferCount{ 4 };
		constexpr uint32 descriptorWriteTextureCount{ 3 };

		m_DescriptorSetWriteBufferProperties.resize(descriptorWriteBufferCount);
		m_DescriptorSetWriteTextureProperties.resize(descriptorWriteTextureCount);

		m_DescriptorSetWriteBufferProperties[0].Initialize(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		m_DescriptorSetWriteBufferProperties[1].Initialize(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC);
		m_DescriptorSetWriteBufferProperties[2].Initialize(4, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
		m_DescriptorSetWriteBufferProperties[3].Initialize(5, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

		m_DescriptorSetWriteTextureProperties[0].Initialize(2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
		m_DescriptorSetWriteTextureProperties[1].Initialize(3, VK_DESCRIPTOR_TYPE_SAMPLER);
		m_DescriptorSetWriteTextureProperties[2].Initialize(6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);

		m_ShadowDescriptorSetWriteBufferProperties.Initialize(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
	}

	void VulkanRenderer::UpdateMaterialData()
	{
		for (const auto& subMesh : m_MeshSystem.GetAllSubMeshes())
		{
			const Material material{ subMesh.GetMaterial() };
			Material* materialData{ (Material*)((uint64)m_MaterialDynamicBufferMemBlock.get() + (subMesh.GetMeshId() * m_MaterialDynamicBufferMemAlignment)) };
			*materialData = { material.GetDiffuseColor(), material.GetSpecularColor() };
		}

		for (size_t i = 0; i < m_DescriptorSets.size(); ++i)
		{
			m_MaterialUniformBuffers[i].CopyData(m_MaterialDynamicBufferMemBlock.get());
		}
	}

	void VulkanRenderer::UpdateLightData()
	{
		constexpr uint8 maxLights{ 25 };
		std::array<LightData, maxLights> lights{};
		uint8 currentLightCount{ 0 };

		for (const auto& lightComponent : m_LightSystem.GetLightComponents())
		{
			if (currentLightCount >= maxLights)
			{
				break;
			}

			lightComponent->UpdatePosition();
			lights[currentLightCount++] = lightComponent->GetLightData();
		}

		LightBuffer lightBuffer{};
		lightBuffer.m_TotalLights = currentLightCount;

		std::copy(lights.begin(), lights.begin() + currentLightCount, lightBuffer.m_Lights);

		m_LightUniformBuffers[m_CurrentFrameIndex].CopyData(&lightBuffer);
	}

	void VulkanRenderer::UpdateShadowSceneDescriptorSets()
	{
		std::shared_ptr<DirectionalLightComponent> directionalLightComponent = nullptr;
		for (const auto& lightComponent : m_LightSystem.GetLightComponents())
		{
			if (auto dirLight = std::dynamic_pointer_cast<DirectionalLightComponent>(lightComponent))
			{
				directionalLightComponent = dirLight;
				break;
			}
		}

		const glm::vec3 lightDir = glm::normalize(directionalLightComponent->GetLightData().m_Direction);
		const glm::vec3 lightPos = -lightDir * 10.0f;
		const glm::mat4 lightViewMatrix = glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		const glm::mat4 lightProjMatrix = glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 50.0f);
		m_LightSpaceMatrix = lightProjMatrix * lightViewMatrix;

		m_ShadowUniformBuffer.CopyData(&m_LightSpaceMatrix);
		m_ShadowDescriptorSetWriteBufferProperties.SetBuffer(m_ShadowUniformBuffer.GetBuffer(), m_ShadowUniformBuffer.GetBufferSize());
		m_DescriptorSetShadow.UpdateDescriptorSet({ m_ShadowDescriptorSetWriteBufferProperties });
	}

	void VulkanRenderer::UpdateSceneDescriptorSets(const uint8 _descriptorSetIndex)
	{
		m_DescriptorSetWriteBufferProperties[0].SetBuffer(m_VPUniformBuffers[_descriptorSetIndex].GetBuffer(), m_VPUniformBuffers[_descriptorSetIndex].GetBufferSize());
		m_DescriptorSetWriteBufferProperties[1].SetBuffer(m_MaterialUniformBuffers[_descriptorSetIndex].GetBuffer(), m_MaterialDynamicBufferMemAlignment);
		m_DescriptorSetWriteBufferProperties[2].SetBuffer(m_LightUniformBuffers[_descriptorSetIndex].GetBuffer(), m_LightUniformBuffers[_descriptorSetIndex].GetBufferSize());
		m_DescriptorSetWriteBufferProperties[3].SetBuffer(m_ShadowUniformBuffer.GetBuffer(), m_ShadowUniformBuffer.GetBufferSize());
		m_DescriptorSets[_descriptorSetIndex].UpdateDescriptorSet(m_DescriptorSetWriteBufferProperties);

		// Update uniform buffer with the ViewProjMatrix
		ViewProjMatrix viewProjMatrix{ m_Camera.GetViewProjMatrix() };
		viewProjMatrix.m_Proj[1][1] *= -1.0f;
		m_VPUniformBuffers[m_CurrentFrameIndex].CopyData(&viewProjMatrix);

		UpdateLightData();
	}

	void VulkanRenderer::StaticUpdateDescriptorSets() noexcept
	{
		m_DescriptorSetWriteTextureProperties[0].SetImageView(m_TextureManager.GetTextureImageViews());
		m_DescriptorSetWriteTextureProperties[1].SetSampler(m_TextureSampler.Get());
		m_DescriptorSetWriteTextureProperties[2].SetImageView({ m_DepthBufferShadow.GetImageView() });
		m_DescriptorSetWriteTextureProperties[2].SetSampler(m_TextureSampler.Get());

		for (size_t i = 0; i < m_DescriptorSets.size(); ++i)
		{
			m_DescriptorSets[i].UpdateDescriptorSet(m_DescriptorSetWriteTextureProperties);
		}
	}

	void VulkanRenderer::DrawFrame(const double _deltaTime)
	{
		uint32 imgIndex{ 0 };

		m_FrameResources.GetInFlightFences().Wait(m_CurrentFrameIndex);

		const VkResult result{ vkAcquireNextImageKHR
		(
			m_RenderContext.GetDevice().GetLogicalDevice(),
			m_RenderContext.GetSwapchain().Get(),
			UINT64_MAX,
			m_FrameResources.GetSemaphore().Get()[m_CurrentFrameIndex].first,
			VK_NULL_HANDLE,
			&imgIndex
		) };

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			vkDeviceWaitIdle(m_RenderContext.GetDevice().GetLogicalDevice());

			const uint32 newWidth{ m_Window.GetWidth() };
			const uint32 newHeight{ m_Window.GetHeight() };

			if (newWidth == 0 || newHeight == 0)
			{
				return;
			}

			m_RenderContext.RecreateSwapchain(newWidth, newHeight);
			m_DepthBufferScene.RecreateDepthBuffer(newWidth, newHeight, 0);
			//m_VkShadowDepthBuffer.RecreateDepthBuffer(2048, 2048, VK_IMAGE_USAGE_SAMPLED_BIT);
			//m_VkFramebuffers.RecreateFramebuffers(newWidth, newHeight, m_RenderContext.GetSwapchain().GetImageViews(), m_VkSceneDepthBuffer.GetImageView());
			//m_VkShadowFramebuffer.RecreateFramebuffers(newWidth, newHeight, {}, m_VkShadowDepthBuffer.GetImageView());
			vkResetCommandPool(m_RenderContext.GetDevice().GetLogicalDevice(), m_CommandPool.Get(), 0);
			return;
		}

		m_FrameResources.GetInFlightFences().Reset(m_CurrentFrameIndex);

		m_Camera.ProcessInput(_deltaTime);

		VkSemaphore waitSemaphore{ m_FrameResources.GetSemaphore().Get()[m_CurrentFrameIndex].first };
		VkSemaphore signalSemaphore{ m_FrameResources.GetSemaphore().Get()[m_CurrentFrameIndex].second };

		VkCommandBuffer cmdBuffer{ m_FrameResources.GetCommandBuffers().Get()[imgIndex] };

		vkResetCommandBuffer(cmdBuffer, 0);
		RecordRenderCommands(imgIndex);

		// Submit the command buffer and signal the next semaphore
		m_FrameResources.GetCommandBuffers().Submit
		(
			imgIndex,
			m_RenderContext.GetDevice().GetGraphicsQueue(),
			waitSemaphore,
			signalSemaphore,
			m_FrameResources.GetInFlightFences().Get()[m_CurrentFrameIndex],
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

	void VulkanRenderer::RecordRenderCommands(const uint8 _imgIndex)
	{
		const VkCommandBuffer cmdBuffer{ m_FrameResources.GetCommandBuffers().Get()[_imgIndex] };
		m_FrameResources.GetCommandBuffers().Begin(_imgIndex);

		RenderShadowMap(cmdBuffer);

		UpdateSceneDescriptorSets(_imgIndex);
		RenderScene(cmdBuffer, _imgIndex);

		m_FrameResources.GetCommandBuffers().End(_imgIndex);
	}

	void VulkanRenderer::RenderShadowMap(const VkCommandBuffer& _cmdBuffer)
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_RenderPassManager.GetRenderPass(RenderPassType::DepthOnly)->Get();
		renderPassInfo.framebuffer = m_FrameResources.GetShadowFramebuffer().GetFramebuffer();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VkExtent2D({ g_ShadowWidth, g_ShadowHeight });

		constexpr VkClearDepthStencilValue clearDepthStencil{ 1.0f, 0 };

		std::array<VkClearValue, 1> clearAttachments{};
		clearAttachments[0].depthStencil = clearDepthStencil;
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = clearAttachments.data();

		vkCmdBeginRenderPass(_cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(g_ShadowWidth);
		viewport.height = static_cast<float>(g_ShadowHeight);
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(_cmdBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VkExtent2D({ g_ShadowWidth, g_ShadowHeight });
		vkCmdSetScissor(_cmdBuffer, 0, 1, &scissor);

		for (const auto& subMesh : m_MeshSystem.GetAllSubMeshes())
		{
			const glm::mat4 entityModelMatrix{ m_TransformationSystem.GetEntityModelMatrix(subMesh.GetEntityId()) };

			// Bind vertex & index buffers
			const VulkanVertexBuffer* const vertexBuffer{ m_MeshSystem.GetVertexBuffer(subMesh.GetVertexBufferId()) };
			const VkDeviceSize indexOffset{ subMesh.GetIndexOffset() * sizeof(uint32) };
			vertexBuffer->Bind(_cmdBuffer, indexOffset);

			// Bind graphics pipeline
			auto pipeline{ m_PipelineManager.GetPipeline(PipelineType::DepthOnly) };
			vkCmdBindPipeline(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->Get());

			// Bind descriptor set
			auto currentDescriptorSet{ m_DescriptorSetShadow.Get() };
			vkCmdBindDescriptorSets(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetLayout(), 0, 1, &m_DescriptorSetShadow.Get(), 0, nullptr);

			// Push constants
			const glm::mat4& modelMatrix{ entityModelMatrix * subMesh.GetModelMatrix() };
			vkCmdPushConstants(_cmdBuffer, pipeline->GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(glm::mat4), &modelMatrix);

			vkCmdDrawIndexed(_cmdBuffer, subMesh.GetIndexCount(), 1, 0, 0, 0);
		}

		vkCmdEndRenderPass(_cmdBuffer);
	}

	void VulkanRenderer::RenderScene(const VkCommandBuffer& _cmdBuffer, const uint8 _imgIndex)
	{
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_RenderPassManager.GetRenderPass(RenderPassType::Scene)->Get();
		renderPassInfo.framebuffer = m_FrameResources.GetFramebuffers()[_imgIndex].GetFramebuffer();
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VkExtent2D({ m_RenderContext.GetSwapchain().GetWidth(), m_RenderContext.GetSwapchain().GetHeight() });

		// Clear attachments
		constexpr VkClearColorValue clearColor{ 0.1f, 0.1f, 0.1f, 1.0f };
		constexpr VkClearDepthStencilValue clearDepthStencil{ 1.0f, 0 };

		std::array<VkClearValue, 2> clearAttachments{};
		clearAttachments[0].color = clearColor;
		clearAttachments[1].depthStencil = clearDepthStencil;
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
			auto pipeline{ m_PipelineManager.GetPipeline(PipelineType::Scene) };
			vkCmdBindPipeline(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->Get());

			// Bind descriptor set
			const uint32 dynamicOffset{ static_cast<uint32>(m_MaterialDynamicBufferMemAlignment) * subMesh.GetMeshId() };
			auto currentDescriptorSet{ m_DescriptorSets[_imgIndex].Get() };
			vkCmdBindDescriptorSets(_cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->GetLayout(), 0, 1, &currentDescriptorSet, 1, &dynamicOffset);

			// Push constants
			const glm::mat4& modelMatrix{ entityModelMatrix * subMesh.GetModelMatrix() };
			const PushConstant pc{ modelMatrix, m_Camera.GetPosition(), subMesh.GetTexId() };
			vkCmdPushConstants(_cmdBuffer, pipeline->GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &pc);

			vkCmdDrawIndexed(_cmdBuffer, subMesh.GetIndexCount(), 1, 0, 0, 0);
		}

		vkCmdEndRenderPass(_cmdBuffer);
	}
} // End of namespace