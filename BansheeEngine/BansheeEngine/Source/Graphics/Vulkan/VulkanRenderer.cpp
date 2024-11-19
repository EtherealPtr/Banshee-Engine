#include "VulkanRenderer.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/Entity/EntityManager.h"
#include "Graphics/Components/Transform/TransformComponent.h"
#include "Graphics/Components/Light/DirectionalLightComponent.h"
#include "Graphics/Components/Light/PointLightComponent.h"
#include "Graphics/Components/Light/SpotLightComponent.h"
#include "Graphics/Components/Mesh/PrimitiveMeshComponent.h"
#include "Graphics/Components/Mesh/CustomMeshComponent.h"
#include "Graphics/MeshData.h"
#include "Graphics/Window.h"
#include <array>
#include <algorithm>
#include <vulkan/vulkan.h>

namespace Banshee
{
	constexpr static uint64 g_MaxEntities{ 512 };

	VulkanRenderer::VulkanRenderer(const Window& _window) :
		m_Window(_window),
		m_VkInstance{},
		m_VkSurface{ _window.GetWindow(), m_VkInstance.Get() },
		m_VkDevice{ m_VkInstance.Get(), m_VkSurface.Get() },
		m_VkSwapchain{ m_VkDevice.GetLogicalDevice(), m_VkDevice.GetPhysicalDevice(), m_VkSurface.Get(), _window.GetWidth(), _window.GetHeight() },
		m_DepthBuffer{ m_VkDevice.GetLogicalDevice(), m_VkDevice.GetPhysicalDevice(), m_VkSwapchain.GetWidth(), m_VkSwapchain.GetHeight() },
		m_VkRenderPass{ m_VkDevice.GetLogicalDevice(), m_VkSwapchain.GetFormat(), static_cast<uint32>(m_DepthBuffer.GetFormat()) },
		m_VkCommandPool{ m_VkDevice.GetLogicalDevice(), m_VkDevice.GetQueueIndices().m_GraphicsQueueFamilyIndex },
		m_VkCommandBuffers{ m_VkDevice.GetLogicalDevice(), m_VkCommandPool.Get(), static_cast<uint16>(m_VkSwapchain.GetImageViews().size()) },
		m_VkFramebuffers{ m_VkDevice.GetLogicalDevice(), m_VkRenderPass.Get(), m_VkSwapchain.GetImageViews(), m_DepthBuffer.GetImageView(), m_VkSwapchain.GetWidth(), m_VkSwapchain.GetHeight() },
		m_VkSemaphores{ m_VkDevice.GetLogicalDevice(), static_cast<uint16>(m_VkSwapchain.GetImageViews().size()) },
		m_VkInFlightFences{ m_VkDevice.GetLogicalDevice(), static_cast<uint16>(m_VkSwapchain.GetImageViews().size()) },
		m_VkTextureSampler{ m_VkDevice.GetLogicalDevice(), m_VkDevice.GetPhysicalDevice() },
		m_VkTextureManager{ m_VkDevice.GetLogicalDevice(), m_VkDevice.GetPhysicalDevice(), m_VkDevice.GetGraphicsQueue(), m_VkCommandPool.Get() },
		m_VkDescriptorSetLayout{ m_VkDevice.GetLogicalDevice() },
		m_VkDescriptorPool{ m_VkDevice.GetLogicalDevice(), static_cast<uint16>(m_VkSwapchain.GetImageViews().size()) },
		m_VkGraphicsPipelineManager{ m_VkDevice.GetLogicalDevice(), m_VkRenderPass.Get(), m_VkDescriptorSetLayout.Get(), m_VkSwapchain.GetWidth(), m_VkSwapchain.GetHeight() },
		m_Camera{ 80.0f, static_cast<float>(_window.GetWidth()) / _window.GetHeight(), 0.1f, 100.0f, _window.GetWindow() },
		m_MeshSystem{ m_VkDevice.GetLogicalDevice(), m_VkDevice.GetPhysicalDevice(), m_VkCommandPool.Get(), m_VkDevice.GetGraphicsQueue() },
		m_LightSystem{},
		m_CurrentFrameIndex{ 0 },
		m_MaterialDynamicBufferMemAlignment{ 0 },
		m_MaterialDynamicBufferMemBlock{ nullptr, [](Material* _ptr) noexcept { _aligned_free(_ptr); } }
	{
		InitializeGraphicsComponents();
		AllocateDynamicBufferSpace();
		CreateDescriptorSetWriteBufferProperties();

		const size_t numOfSwapImages{ m_VkSwapchain.GetImageViews().size() };
		m_VPUniformBuffers.reserve(numOfSwapImages);
		m_MaterialUniformBuffers.reserve(numOfSwapImages);
		m_LightUniformBuffers.reserve(numOfSwapImages);
		m_DescriptorSets.reserve(numOfSwapImages);

		for (size_t i = 0; i < numOfSwapImages; ++i)
		{
			m_VPUniformBuffers.emplace_back(m_VkDevice.GetLogicalDevice(), m_VkDevice.GetPhysicalDevice(), sizeof(ViewProjMatrix), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			m_MaterialUniformBuffers.emplace_back(m_VkDevice.GetLogicalDevice(), m_VkDevice.GetPhysicalDevice(), m_MaterialDynamicBufferMemAlignment * g_MaxEntities, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			m_LightUniformBuffers.emplace_back(m_VkDevice.GetLogicalDevice(), m_VkDevice.GetPhysicalDevice(), sizeof(LightBuffer), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
			m_DescriptorSets.emplace_back(m_VkDevice.GetLogicalDevice(), m_VkDescriptorPool.Get(), m_VkDescriptorSetLayout.Get());
		}

		UpdateMaterialData();
		m_VkTextureManager.UploadTextures();
		StaticUpdateDescriptorSets();
		BE_LOG(LogCategory::Trace, "[RENDERER]: Vulkan initialized");
	}

	VulkanRenderer::~VulkanRenderer()
	{
		vkDeviceWaitIdle(m_VkDevice.GetLogicalDevice());
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
		const VkDeviceSize minUniformBufferOffset{ m_VkDevice.GetLimits().minUniformBufferOffsetAlignment };
		m_MaterialDynamicBufferMemAlignment = (sizeof(Material) + minUniformBufferOffset - 1) & ~(m_VkDevice.GetLimits().minUniformBufferOffsetAlignment - 1);
		m_MaterialDynamicBufferMemBlock.reset(static_cast<Material*>(_aligned_malloc(m_MaterialDynamicBufferMemAlignment * g_MaxEntities, m_MaterialDynamicBufferMemAlignment)));
	}

	void VulkanRenderer::CreateDescriptorSetWriteBufferProperties()
	{
		constexpr uint32 descriptorWriteBufferCount{ 3 };
		constexpr uint32 descriptorWriteTextureCount{ 2 };

		m_DescriptorSetWriteBufferProperties.resize(descriptorWriteBufferCount);
		m_DescriptorSetWriteTextureProperties.resize(descriptorWriteTextureCount);

		m_DescriptorSetWriteBufferProperties[0].Initialize(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		m_DescriptorSetWriteBufferProperties[1].Initialize(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC);
		m_DescriptorSetWriteBufferProperties[2].Initialize(4, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);

		m_DescriptorSetWriteTextureProperties[0].Initialize(2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
		m_DescriptorSetWriteTextureProperties[1].Initialize(3, VK_DESCRIPTOR_TYPE_SAMPLER);
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

	void VulkanRenderer::UpdateDescriptorSets(const uint8 _descriptorSetIndex)
	{
		m_DescriptorSetWriteBufferProperties[0].SetBuffer(m_VPUniformBuffers[_descriptorSetIndex].GetBuffer(), m_VPUniformBuffers[_descriptorSetIndex].GetBufferSize());
		m_DescriptorSetWriteBufferProperties[1].SetBuffer(m_MaterialUniformBuffers[_descriptorSetIndex].GetBuffer(), m_MaterialDynamicBufferMemAlignment);
		m_DescriptorSetWriteBufferProperties[2].SetBuffer(m_LightUniformBuffers[_descriptorSetIndex].GetBuffer(), m_LightUniformBuffers[_descriptorSetIndex].GetBufferSize());
		m_DescriptorSets[_descriptorSetIndex].UpdateDescriptorSet(m_DescriptorSetWriteBufferProperties);

		// Update uniform buffer with the ViewProjMatrix
		ViewProjMatrix viewProjMatrix{ m_Camera.GetViewProjMatrix() };
		viewProjMatrix.m_Proj[1][1] *= -1.0f;
		m_VPUniformBuffers[m_CurrentFrameIndex].CopyData(&viewProjMatrix);

		UpdateLightData();
	}

	void VulkanRenderer::StaticUpdateDescriptorSets() noexcept
	{
		m_DescriptorSetWriteTextureProperties[0].SetImageView(m_VkTextureManager.GetTextureImageViews());
		m_DescriptorSetWriteTextureProperties[1].SetSampler(m_VkTextureSampler.Get());

		for (size_t i = 0; i < m_DescriptorSets.size(); ++i)
		{
			m_DescriptorSets[i].UpdateDescriptorSet(m_DescriptorSetWriteTextureProperties);
		}
	}

	void VulkanRenderer::DrawFrame(const double _deltaTime)
	{
		uint32 imgIndex{ 0 };

		m_VkInFlightFences.Wait(m_CurrentFrameIndex);

		const VkResult result{ vkAcquireNextImageKHR
		(
			m_VkDevice.GetLogicalDevice(),
			m_VkSwapchain.Get(),
			UINT64_MAX,
			m_VkSemaphores.Get()[m_CurrentFrameIndex].first,
			VK_NULL_HANDLE,
			&imgIndex
		) };

		if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
		{
			vkDeviceWaitIdle(m_VkDevice.GetLogicalDevice());

			const uint32 newWidth{ m_Window.GetWidth() };
			const uint32 newHeight{ m_Window.GetHeight() };

			if (newWidth == 0 || newHeight == 0)
			{
				return;
			}

			m_VkSwapchain.RecreateSwapchain(newWidth, newHeight);
			m_DepthBuffer.RecreateDepthBuffer(newWidth, newHeight);
			m_VkFramebuffers.RecreateFramebuffers(newWidth, newHeight, m_VkSwapchain.GetImageViews(), m_DepthBuffer.GetImageView());
			vkResetCommandPool(m_VkDevice.GetLogicalDevice(), m_VkCommandPool.Get(), 0);
			return;
		}

		m_VkInFlightFences.Reset(m_CurrentFrameIndex);

		// Update the camera's position and rotation
		m_Camera.ProcessInput(_deltaTime);

		// Get the semaphores to use for this frame
		VkSemaphore waitSemaphore{ m_VkSemaphores.Get()[m_CurrentFrameIndex].first };
		VkSemaphore signalSemaphore{ m_VkSemaphores.Get()[m_CurrentFrameIndex].second };

		// Get the command buffer for this image index
		VkCommandBuffer cmdBuffer{ m_VkCommandBuffers.Get()[imgIndex] };

		// Reset the command buffer and record render commands
		vkResetCommandBuffer(cmdBuffer, 0);
		RecordRenderCommands(imgIndex);

		// Submit the command buffer and signal the next semaphore
		m_VkCommandBuffers.Submit
		(
			imgIndex,
			m_VkDevice.GetGraphicsQueue(),
			waitSemaphore,
			signalSemaphore,
			m_VkInFlightFences.Get()[m_CurrentFrameIndex],
			VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT
		);

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

	void VulkanRenderer::RecordRenderCommands(const uint8 _imgIndex)
	{
		const VkCommandBuffer cmdBuffer{ m_VkCommandBuffers.Get()[_imgIndex] };
		m_VkCommandBuffers.Begin(_imgIndex);

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_VkRenderPass.Get();
		renderPassInfo.framebuffer = m_VkFramebuffers.Get()[_imgIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VkExtent2D({ m_VkSwapchain.GetWidth(), m_VkSwapchain.GetHeight() });

		// Clear attachments
		constexpr VkClearColorValue clearColor{ 0.1f, 0.1f, 0.1f, 1.0f };
		constexpr VkClearDepthStencilValue clearDepthStencil{ 1.0f, 0 };

		std::array<VkClearValue, 2> clearAttachments{};
		clearAttachments[0].color = clearColor;
		clearAttachments[1].depthStencil = clearDepthStencil;
		renderPassInfo.clearValueCount = static_cast<uint32>(clearAttachments.size());
		renderPassInfo.pClearValues = clearAttachments.data();

		vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

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

		UpdateDescriptorSets(_imgIndex);

		for (const auto& subMesh : m_MeshSystem.GetAllSubMeshes())
		{
			const glm::mat4 entityModelMatrix{ m_TransformationSystem.GetEntityModelMatrix(subMesh.GetEntityId()) };

			// Bind vertex & index buffers
			const VulkanVertexBuffer* const vertexBuffer{ m_MeshSystem.GetVertexBuffer(subMesh.GetVertexBufferId()) };
			const VkDeviceSize indexOffset{ subMesh.GetIndexOffset() * sizeof(uint32) };
			vertexBuffer->Bind(cmdBuffer, indexOffset);

			// Bind graphics pipeline
			const auto& graphicsPipeline{ m_VkGraphicsPipelineManager.GetPipeline(subMesh.GetMaterial().GetShaderType()) };
			vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->Get());

			// Bind descriptor set
			const uint32 dynamicOffset{ static_cast<uint32>(m_MaterialDynamicBufferMemAlignment) * subMesh.GetMeshId() };
			auto currentDescriptorSet{ m_DescriptorSets[_imgIndex].Get() };
			vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->GetLayout(), 0, 1, &currentDescriptorSet, 1, &dynamicOffset);

			// Push constants
			const glm::mat4& modelMatrix{ entityModelMatrix * subMesh.GetModelMatrix() };
			const PushConstant pc{ modelMatrix, m_Camera.GetPosition(), subMesh.GetTexId() };
			vkCmdPushConstants(cmdBuffer, graphicsPipeline->GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &pc);

			vkCmdDrawIndexed(cmdBuffer, subMesh.GetIndexCount(), 1, 0, 0, 0);
		}

		vkCmdEndRenderPass(cmdBuffer);
		m_VkCommandBuffers.End(_imgIndex);
	}
} // End of Banshee namespace