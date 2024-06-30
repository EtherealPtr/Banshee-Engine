#include "VulkanRenderer.h"
#include "VulkanInstance.h"
#include "VulkanSurface.h"
#include "VulkanDevice.h"
#include "VulkanSwapchain.h"
#include "VulkanDepthBuffer.h"
#include "VulkanRenderPass.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSet.h"
#include "VulkanGraphicsPipeline.h"
#include "VulkanGraphicsPipelineManager.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFramebuffer.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanUniformBuffer.h"
#include "VulkanDescriptorSetProperties.h"
#include "VulkanTextureManager.h"
#include "VulkanTextureSampler.h"
#include "VulkanVertexBufferManager.h"
#include "Foundation/Entity/EntityManager.h"
#include "Graphics/Systems/MeshSystem.h"
#include "Graphics/Systems/LightSystem.h"
#include "Foundation/Logging/Logger.h"
#include "Graphics/Components/TransformComponent.h"
#include "Graphics/Components/Light/LightComponent.h"
#include "Graphics/Components/MeshComponent.h"
#include "Graphics/Material.h"
#include "Graphics/Window.h"
#include "Graphics/Camera.h"
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <array>
#include <algorithm>

namespace Banshee
{
	constexpr uint64 g_MaxEntities = 512;

	VulkanRenderer::VulkanRenderer(const Window* _window) :
		m_VkInstance(std::make_unique<VulkanInstance>()),
		m_VkSurface(std::make_unique<VulkanSurface>(_window->GetWindow(), m_VkInstance->Get())),
		m_VkDevice(std::make_unique<VulkanDevice>(m_VkInstance->Get(), m_VkSurface->Get())),
		m_VkSwapchain(std::make_unique<VulkanSwapchain>(m_VkDevice->GetLogicalDevice(), m_VkDevice->GetPhysicalDevice(), m_VkSurface->Get(), _window->GetWidth(), _window->GetHeight())),
		m_DepthBuffer(std::make_unique<VulkanDepthBuffer>(m_VkDevice->GetLogicalDevice(), m_VkDevice->GetPhysicalDevice(), m_VkSwapchain->GetWidth(), m_VkSwapchain->GetHeight())),
		m_VkRenderPass(std::make_unique<VulkanRenderPass>(m_VkDevice->GetLogicalDevice(), m_VkSwapchain->GetFormat(), m_DepthBuffer->GetFormat())),
		m_VkCommandPool(std::make_unique<VulkanCommandPool>(m_VkDevice->GetLogicalDevice(), m_VkDevice->GetQueueIndices().graphicsQueueFamilyIndex)),
		m_VkCommandBuffers(std::make_unique<VulkanCommandBuffer>(m_VkDevice->GetLogicalDevice(), m_VkCommandPool->Get(), static_cast<uint16>(m_VkSwapchain->GetImageViews().size()))),
		m_VkFramebuffers(std::make_unique<VulkanFramebuffer>(m_VkDevice->GetLogicalDevice(), m_VkRenderPass->Get(), m_VkSwapchain->GetImageViews(), m_DepthBuffer->GetImageView(), m_VkSwapchain->GetWidth(), m_VkSwapchain->GetHeight())),
		m_VkSemaphores(std::make_unique<VulkanSemaphore>(m_VkDevice->GetLogicalDevice(), static_cast<uint16>(m_VkSwapchain->GetImageViews().size()))),
		m_VkInFlightFences(std::make_unique<VulkanFence>(m_VkDevice->GetLogicalDevice(), static_cast<uint16>(m_VkSwapchain->GetImageViews().size()))),
		m_VertexBufferManager(std::make_unique<VulkanVertexBufferManager>(m_VkDevice->GetLogicalDevice(), m_VkDevice->GetPhysicalDevice(), m_VkCommandPool->Get(), m_VkDevice->GetGraphicsQueue())),
		m_VkTextureSampler(std::make_unique<VulkanTextureSampler>(m_VkDevice->GetLogicalDevice(), m_VkDevice->GetPhysicalDevice())),
		m_VkTextureManager(std::make_unique<VulkanTextureManager>(m_VkDevice->GetLogicalDevice(), m_VkDevice->GetPhysicalDevice(), m_VkDevice->GetGraphicsQueue(), m_VkCommandPool->Get())),
		m_VkDescriptorSetLayout(std::make_unique<VulkanDescriptorSetLayout>(m_VkDevice->GetLogicalDevice())),
		m_VkDescriptorPool(std::make_unique<VulkanDescriptorPool>(m_VkDevice->GetLogicalDevice(), static_cast<uint16>(m_VkSwapchain->GetImageViews().size()))),
		m_VkGraphicsPipelineManager(std::make_unique<VulkanGraphicsPipelineManager>(m_VkDevice->GetLogicalDevice(), m_VkRenderPass->Get(), m_VkDescriptorSetLayout->Get(), m_VkSwapchain->GetWidth(), m_VkSwapchain->GetHeight())),
		m_Camera(std::make_unique<Camera>(45.0f, static_cast<float>(_window->GetWidth()) / _window->GetHeight(), 0.1f, 100.0f))
	{
		FetchGraphicsComponents();
		AllocateDynamicBufferSpace();

		const size_t numOfSwapImages = m_VkSwapchain->GetImageViews().size();
		m_VPUniformBuffers.reserve(numOfSwapImages);
		m_MaterialUniformBuffers.reserve(numOfSwapImages);
		m_LightUniformBuffers.reserve(numOfSwapImages);
		m_DescriptorSets.reserve(numOfSwapImages);

		for (size_t i = 0; i < numOfSwapImages; ++i)
		{
			// Create view-proj UBO
			m_VPUniformBuffers.emplace_back(std::make_unique<VulkanUniformBuffer>(m_VkDevice->GetLogicalDevice(), m_VkDevice->GetPhysicalDevice(), sizeof(ViewProjMatrix)));

			// Create Dynamic Material UBO
			m_MaterialUniformBuffers.emplace_back(std::make_unique<VulkanUniformBuffer>(m_VkDevice->GetLogicalDevice(), m_VkDevice->GetPhysicalDevice(), m_MaterialDynamicBufferMemAlignment * g_MaxEntities));

			// Create Light UBO
			m_LightUniformBuffers.emplace_back(std::make_unique<VulkanUniformBuffer>(m_VkDevice->GetLogicalDevice(), m_VkDevice->GetPhysicalDevice(), sizeof(LightData)));

			// Create descriptor sets
			m_DescriptorSets.emplace_back(std::make_unique<VulkanDescriptorSet>(m_VkDevice->GetLogicalDevice(), m_VkDescriptorPool->Get(), m_VkDescriptorSetLayout->Get()));
		}

		m_CurrentFrameIndex = 0;
		for (const auto& meshComponent : MeshSystem::Instance().GetMeshComponents())
		{
			meshComponent->SetDirty(true);

			if (meshComponent->HasModel())
			{
				m_VertexBufferManager->CreateModelVertexBuffer(meshComponent.get());
			}
			else
			{
				m_VertexBufferManager->CreateBasicShapeVertexBuffer(meshComponent.get());
			}
		}

		RegisterLightObservers();
		UpdateMaterialData();
		m_VkTextureManager->UploadTextures();
		BE_LOG(LogCategory::Trace, "[RENDERER]: Vulkan initialized");
	}

	VulkanRenderer::~VulkanRenderer() noexcept
	{
		vkDeviceWaitIdle(m_VkDevice->GetLogicalDevice());
		_aligned_free(m_MaterialDynamicBufferMemBlock);
	}

	void VulkanRenderer::FetchGraphicsComponents() const
	{
		std::vector<std::shared_ptr<MeshComponent>> meshComponents{};
		std::vector<std::shared_ptr<LightComponent>> lightComponents{};

		for (const auto& entity : EntityManager::GetAllEntities())
		{
			if (const auto& meshComponent = entity->GetComponent<MeshComponent>())
			{
				meshComponents.push_back(meshComponent);
			}

			if (const auto& lightComponent = entity->GetComponent<LightComponent>())
			{
				lightComponents.push_back(lightComponent);
			}
		}

		// Sort mesh components based on shader type
		std::sort(meshComponents.begin(), meshComponents.end(), [](const std::shared_ptr<MeshComponent>& _a, const std::shared_ptr<MeshComponent>& _b) noexcept
		{
			return _a->GetShaderType() < _b->GetShaderType();
		});

		MeshSystem::Instance().SetMeshComponents(meshComponents);
		LightSystem::Instance().SetLightComponents(lightComponents);
	}

	void VulkanRenderer::AllocateDynamicBufferSpace()
	{
		const VkDeviceSize minUniformBufferOffset = m_VkDevice->GetLimits().minUniformBufferOffsetAlignment;
		m_MaterialDynamicBufferMemAlignment = (sizeof(Material) + minUniformBufferOffset - 1) & ~(m_VkDevice->GetLimits().minUniformBufferOffsetAlignment - 1);
		m_MaterialDynamicBufferMemBlock = static_cast<Material*>(_aligned_malloc(m_MaterialDynamicBufferMemAlignment * g_MaxEntities, m_MaterialDynamicBufferMemAlignment));
	}

	void VulkanRenderer::RegisterLightObservers() noexcept
	{
		const auto& lightComponents = LightSystem::Instance().GetLightComponents();
		for (const auto& lightComponent : lightComponents)
		{
			if (auto transformComponent = lightComponent->GetOwner()->GetTransform())
			{
				transformComponent->RegisterObserver(lightComponent);
			}
		}
	}

	void VulkanRenderer::UpdateMaterialData()
	{
		// Update dynamic buffer with material data
		for (const auto& meshComponent : MeshSystem::Instance().GetMeshComponents())
		{
			for (const auto& subMesh : meshComponent->GetSubMeshes())
			{
				auto material = subMesh.material;
				Material* materialData = (Material*)((uint64)m_MaterialDynamicBufferMemBlock + (subMesh.GetMaterialIndex() * m_MaterialDynamicBufferMemAlignment));
				const glm::vec3 diffuseColor = material.GetDiffuseColor();
				const glm::vec3 specularColor = material.GetSpecularColor();
				const float shininess = material.GetShininess();
				*materialData = { diffuseColor, specularColor, shininess };
			}
		}

		for (uint8 i = 0; i < m_DescriptorSets.size(); ++i)
		{
			m_MaterialUniformBuffers[i]->CopyData(m_MaterialDynamicBufferMemBlock);
		}
	}

	void VulkanRenderer::UpdateLightData()
	{
		const auto& lightComponents = LightSystem::Instance().GetLightComponents();
		for (const auto& lightComponent : lightComponents)
		{
			if (auto transformComponent = lightComponent->GetOwner()->GetTransform())
			{
				LightData lightData(transformComponent->GetPosition(), lightComponent->GetColor());
				m_LightUniformBuffers[m_CurrentFrameIndex]->CopyData(&lightData);
			}
		}
	}

	void VulkanRenderer::UpdateDescriptorSets(const uint8 _descriptorSetIndex)
	{
		const DescriptorSetWriteBufferProperties viewProjBufferDescWriteProperties(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, m_VPUniformBuffers[_descriptorSetIndex]->GetBuffer(), m_VPUniformBuffers[_descriptorSetIndex]->GetBufferSize());
		const DescriptorSetWriteBufferProperties materialDynamicBufferDescWriteProperties(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, m_MaterialUniformBuffers[_descriptorSetIndex]->GetBuffer(), m_MaterialDynamicBufferMemAlignment);
		const DescriptorSetWriteTextureProperties texturesDescWriteProperties(2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, m_VkTextureManager->GetTextureImageViews(), VK_NULL_HANDLE);
		const DescriptorSetWriteTextureProperties samplerDescWriteProperties(3, VK_DESCRIPTOR_TYPE_SAMPLER, {}, m_VkTextureSampler->Get());
		const DescriptorSetWriteBufferProperties lightBufferDescWriteProperties(4, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, m_LightUniformBuffers[_descriptorSetIndex]->GetBuffer(), m_LightUniformBuffers[_descriptorSetIndex]->GetBufferSize());
		
		const std::vector<DescriptorSetWriteBufferProperties> descriptorSetWriteBufferProperties =
		{
			viewProjBufferDescWriteProperties,
			materialDynamicBufferDescWriteProperties,
			lightBufferDescWriteProperties
		};

		m_DescriptorSets[_descriptorSetIndex]->UpdateDescriptorSet(descriptorSetWriteBufferProperties);

		const std::vector<DescriptorSetWriteTextureProperties> descriptorSetWriteTextureProperties =
		{
			texturesDescWriteProperties,
			samplerDescWriteProperties
		};

		m_DescriptorSets[_descriptorSetIndex]->UpdateDescriptorSet(descriptorSetWriteTextureProperties);

		// Update uniform buffer with the ViewProjMatrix
		ViewProjMatrix viewProjMatrix = m_Camera->GetViewProjMatrix();
		viewProjMatrix.proj[1][1] *= -1.0f;
		m_VPUniformBuffers[m_CurrentFrameIndex]->CopyData(&viewProjMatrix);

		UpdateLightData();
	}

	void VulkanRenderer::DrawFrame(const double _deltaTime)
	{
		uint32 imgIndex = 0;

		m_VkInFlightFences->Wait(m_CurrentFrameIndex);
		m_VkInFlightFences->Reset(m_CurrentFrameIndex);

		vkAcquireNextImageKHR(m_VkDevice->GetLogicalDevice(), m_VkSwapchain->Get(), UINT64_MAX,
			m_VkSemaphores->Get()[m_CurrentFrameIndex].first, VK_NULL_HANDLE, &imgIndex);

		// Update the camera's position and rotation
		m_Camera->ProcessInput(_deltaTime);

		// Get the semaphores to use for this frame
		VkSemaphore waitSemaphore = m_VkSemaphores->Get()[m_CurrentFrameIndex].first;
		VkSemaphore signalSemaphore = m_VkSemaphores->Get()[m_CurrentFrameIndex].second;

		// Get the command buffer for this image index
		VkCommandBuffer cmdBuffer = m_VkCommandBuffers->Get()[imgIndex];

		// Reset the command buffer and record render commands
		vkResetCommandBuffer(cmdBuffer, 0);
		RecordRenderCommands(imgIndex);

		// Submit the command buffer and signal the next semaphore
		m_VkCommandBuffers->Submit(imgIndex, m_VkDevice->GetGraphicsQueue(), waitSemaphore, signalSemaphore, m_VkInFlightFences->Get()[m_CurrentFrameIndex], VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT);

		// Present the current image and wait for the current signal semaphore
		VkPresentInfoKHR presentInfo{};
		presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
		presentInfo.waitSemaphoreCount = 1;
		presentInfo.pWaitSemaphores = &signalSemaphore;

		VkSwapchainKHR swapchains[] = { m_VkSwapchain->Get() };
		presentInfo.swapchainCount = 1;
		presentInfo.pSwapchains = swapchains;
		presentInfo.pImageIndices = &imgIndex;
		presentInfo.pResults = nullptr;

		vkQueuePresentKHR(m_VkDevice->GetPresentationQueue(), &presentInfo);

		m_CurrentFrameIndex = (m_CurrentFrameIndex + 1) % m_VkSwapchain->GetImageViews().size();
	}

	void VulkanRenderer::RecordRenderCommands(const uint8 _imgIndex)
	{
		const VkCommandBuffer cmdBuffer = m_VkCommandBuffers->Get()[_imgIndex];
		m_VkCommandBuffers->Begin(_imgIndex);

		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_VkRenderPass->Get();
		renderPassInfo.framebuffer = m_VkFramebuffers->Get()[_imgIndex];
		renderPassInfo.renderArea.offset = { 0, 0 };
		renderPassInfo.renderArea.extent = VkExtent2D({ m_VkSwapchain->GetWidth(), m_VkSwapchain->GetHeight() });

		// Clear attachments
        const VkClearColorValue clearColor = { 0.0f, 0.0f, 0.5f, 1.0f };
		const VkClearDepthStencilValue clearDepthStencil{ 1.0f, 0 };

		std::array<VkClearValue, 2> clearAttachments{};
		clearAttachments[0].color = clearColor;
		clearAttachments[1].depthStencil = clearDepthStencil;
		renderPassInfo.clearValueCount = static_cast<uint32>(clearAttachments.size());
		renderPassInfo.pClearValues = clearAttachments.data();

		vkCmdBeginRenderPass(cmdBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = static_cast<float>(m_VkSwapchain->GetWidth());
		viewport.height = static_cast<float>(m_VkSwapchain->GetHeight());
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;
		vkCmdSetViewport(cmdBuffer, 0, 1, &viewport);

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = VkExtent2D({ m_VkSwapchain->GetWidth(), m_VkSwapchain->GetHeight() });
		vkCmdSetScissor(cmdBuffer, 0, 1, &scissor);

		UpdateDescriptorSets(_imgIndex);

		const std::vector<std::shared_ptr<MeshComponent>>& meshComponents = MeshSystem::Instance().GetMeshComponents();
		for (uint32 i = 0; i < meshComponents.size(); ++i)
		{
            glm::mat4 entityModelMatrix = glm::mat4(1.0f);
            if (auto transform = meshComponents[i]->GetOwner()->GetTransform())
            {
                entityModelMatrix = transform->GetModel();
            }

			const VulkanVertexBuffer* const vertexBuffer = m_VertexBufferManager->GetVertexBuffer(meshComponents[i]->GetMeshId());

			const auto& graphicsPipeline = m_VkGraphicsPipelineManager->GetPipeline(meshComponents[i]->GetShaderType());
			vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->Get());

			for (const auto& subMesh : meshComponents[i]->GetSubMeshes())
			{
				// Bind vertex & index buffers
				const VkDeviceSize indexOffset = subMesh.indexOffset * sizeof(uint32);
				vertexBuffer->Bind(cmdBuffer, indexOffset);

				// Bind descriptor set
				const uint32 dynamicOffset = static_cast<uint32>(m_MaterialDynamicBufferMemAlignment) * subMesh.GetMaterialIndex();
				auto currentDescriptorSet = m_DescriptorSets[_imgIndex]->Get();
				vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline->GetLayout(), 0, 1, &currentDescriptorSet, 1, &dynamicOffset);

				// Push constants
				const glm::mat4& modelMatrix = entityModelMatrix * subMesh.localTransform;
				const PushConstant pc(modelMatrix, subMesh.GetTexId(), subMesh.HasTexture());
				vkCmdPushConstants(cmdBuffer, graphicsPipeline->GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &pc);

				vkCmdDrawIndexed(cmdBuffer, static_cast<uint32>(subMesh.indices.size()), 1, 0, 0, 0);
			}
		}

		vkCmdEndRenderPass(cmdBuffer);
		m_VkCommandBuffers->End(_imgIndex);
	}
} // End of Banshee namespace