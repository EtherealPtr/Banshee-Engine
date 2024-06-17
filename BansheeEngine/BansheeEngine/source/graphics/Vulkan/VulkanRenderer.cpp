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
#include "Foundation/Components/TransformComponent.h"
#include "Graphics/Systems/MeshSystem.h"
#include "Foundation/Logging/Logger.h"
#include "Graphics/Components/MeshComponent.h"
#include "Graphics/Components/Material.h"
#include "Graphics/Window.h"
#include "Graphics/Camera.h"
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <array>

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
		m_VkDescriptorSetLayout(std::make_unique<VulkanDescriptorSetLayout>(m_VkDevice->GetLogicalDevice(), VK_SHADER_STAGE_VERTEX_BIT)),
		m_VkDescriptorPool(std::make_unique<VulkanDescriptorPool>(m_VkDevice->GetLogicalDevice(), static_cast<uint16>(m_VkSwapchain->GetImageViews().size()))),
		m_VkGraphicsPipeline(std::make_unique<VulkanGraphicsPipeline>(m_VkDevice->GetLogicalDevice(), m_VkRenderPass->Get(), m_VkDescriptorSetLayout->Get(), m_VkSwapchain->GetWidth(), m_VkSwapchain->GetHeight())),
		m_Camera(std::make_unique<Camera>(45.0f, static_cast<float>(_window->GetWidth()) / _window->GetHeight(), 0.1f, 100.0f))
	{
		FetchMeshComponents();
		AllocateDynamicBufferSpace();

		const size_t numOfSwapImages = m_VkSwapchain->GetImageViews().size();
		m_VPUniformBuffers.reserve(numOfSwapImages);
		m_DynamicUniformBuffers.reserve(numOfSwapImages);
		m_DescriptorSets.reserve(numOfSwapImages);

		for (size_t i = 0; i < numOfSwapImages; ++i)
		{
			// Create view-proj UBO
			m_VPUniformBuffers.emplace_back(std::make_unique<VulkanUniformBuffer>(m_VkDevice->GetLogicalDevice(), m_VkDevice->GetPhysicalDevice(), sizeof(ViewProjMatrix)));

			// Create Dynamic UBO
			m_DynamicUniformBuffers.emplace_back(std::make_unique<VulkanUniformBuffer>(m_VkDevice->GetLogicalDevice(), m_VkDevice->GetPhysicalDevice(), m_DynamicBufferMemoryAlignment * g_MaxEntities));

			// Create descriptor sets
			m_DescriptorSets.emplace_back(std::make_unique<VulkanDescriptorSet>(m_VkDevice->GetLogicalDevice(), m_VkDescriptorPool->Get(), m_VkDescriptorSetLayout->Get()));
		}

		m_CurrentFrameIndex = 0;
		const std::vector<std::shared_ptr<MeshComponent>>& meshComponents = MeshSystem::Instance().GetMeshComponents();
		for (const auto& meshComponent : meshComponents)
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

		m_VkTextureManager->UploadTextures();
		BE_LOG(LogCategory::Trace, "[RENDERER]: Vulkan initialized");
	}

	VulkanRenderer::~VulkanRenderer() noexcept
	{
		vkDeviceWaitIdle(m_VkDevice->GetLogicalDevice());
		_aligned_free(m_DynamicBufferMemorySpace);
	}

	void VulkanRenderer::AllocateDynamicBufferSpace()
	{
		const VkDeviceSize minUniformBufferOffset = m_VkDevice->GetLimits().minUniformBufferOffsetAlignment;
		m_DynamicBufferMemoryAlignment = (sizeof(Material) + minUniformBufferOffset - 1) & ~(m_VkDevice->GetLimits().minUniformBufferOffsetAlignment - 1);
		m_DynamicBufferMemorySpace = static_cast<Material*>(_aligned_malloc(m_DynamicBufferMemoryAlignment * g_MaxEntities, m_DynamicBufferMemoryAlignment));
	}

	void VulkanRenderer::UpdateDescriptorSet(const uint8 _descriptorSetIndex)
	{
		const DescriptorSetWriteBufferProperties uniformBufferDescWriteProperties(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, m_VPUniformBuffers[_descriptorSetIndex]->GetBuffer(), m_VPUniformBuffers[_descriptorSetIndex]->GetBufferSize());
		const DescriptorSetWriteBufferProperties dynamicUniformBufferDescWriteProperties(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, m_DynamicUniformBuffers[_descriptorSetIndex]->GetBuffer(), m_DynamicBufferMemoryAlignment);
		DescriptorSetWriteTextureProperties texturesDescWriteProperties(2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, m_VkTextureManager->GetTextureImageViews(), VK_NULL_HANDLE);
		DescriptorSetWriteTextureProperties samplerDescWriteProperties(3, VK_DESCRIPTOR_TYPE_SAMPLER, {}, m_VkTextureSampler->Get());
		
		const std::vector<DescriptorSetWriteBufferProperties> descriptorSetWriteBufferProperties =
		{
			uniformBufferDescWriteProperties,
			dynamicUniformBufferDescWriteProperties
		};

		m_DescriptorSets[_descriptorSetIndex]->UpdateDescriptorSet(descriptorSetWriteBufferProperties);

		const std::vector<DescriptorSetWriteTextureProperties> descriptorSetWriteTextureProperties =
		{
			texturesDescWriteProperties,
			samplerDescWriteProperties
		};

		m_DescriptorSets[_descriptorSetIndex]->UpdateDescriptorSet(descriptorSetWriteTextureProperties);

		// Update dynamic buffer with material data
		const std::vector<std::shared_ptr<MeshComponent>>& meshComponents = MeshSystem::Instance().GetMeshComponents();
		size_t dynamicOffsetIndex = 0;

		for (const auto& meshComponent : meshComponents)
		{
			for (const auto& subMesh : meshComponent->GetSubMeshes())
			{
				auto material = subMesh.material;
				Material* materialData = (Material*)((uint64)m_DynamicBufferMemorySpace + (dynamicOffsetIndex * m_DynamicBufferMemoryAlignment));
				const glm::vec3 diffuseColor = material.GetDiffuseColor();
				const glm::vec3 specularColor = material.GetSpecularColor();
				const float shininess = material.GetShininess();
				*materialData = { diffuseColor, specularColor, shininess };
				++dynamicOffsetIndex;
			}
		}

		m_DynamicUniformBuffers[_descriptorSetIndex]->CopyData(m_DynamicBufferMemorySpace);

		// Update uniform buffer with the ViewProjMatrix
		ViewProjMatrix viewProjMatrix = m_Camera->GetViewProjMatrix();
		viewProjMatrix.proj[1][1] *= -1.0f;
		m_VPUniformBuffers[m_CurrentFrameIndex]->CopyData(&viewProjMatrix);
	}

	void VulkanRenderer::DrawFrame()
	{
		unsigned int imgIndex = 0;

		m_VkInFlightFences->Wait(m_CurrentFrameIndex);
		m_VkInFlightFences->Reset(m_CurrentFrameIndex);

		vkAcquireNextImageKHR(m_VkDevice->GetLogicalDevice(), m_VkSwapchain->Get(), UINT64_MAX,
			m_VkSemaphores->Get()[m_CurrentFrameIndex].first, VK_NULL_HANDLE, &imgIndex);

		// Update the camera's position and rotation
		m_Camera->ProcessInput();

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
		vkCmdBindPipeline(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VkGraphicsPipeline->Get());

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

		UpdateDescriptorSet(_imgIndex);

		const std::vector<std::shared_ptr<MeshComponent>>& meshComponents = MeshSystem::Instance().GetMeshComponents();
		for (uint32 i = 0; i < meshComponents.size(); ++i)
		{
			const VulkanVertexBuffer* const vertexBuffer = m_VertexBufferManager->GetVertexBuffer(meshComponents[i]->GetMeshId());

			for (const auto& subMesh : meshComponents[i]->GetSubMeshes())
			{
				// Bind vertex & index buffers
				const VkDeviceSize indexOffset = subMesh.indexOffset * sizeof(uint32);
				vertexBuffer->Bind(cmdBuffer, 0, indexOffset);

				// Bind descriptor set
				const uint32 dynamicOffset = static_cast<uint32>(m_DynamicBufferMemoryAlignment) * subMesh.materialIndex;
				auto currentDescriptorSet = m_DescriptorSets[_imgIndex]->Get();
				vkCmdBindDescriptorSets(cmdBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, m_VkGraphicsPipeline->GetLayout(), 0, 1, &currentDescriptorSet, 1, &dynamicOffset);

				// Push constants
				const glm::mat4& modelMatrix = subMesh.modelMatrix;
				const PushConstant pc(modelMatrix, meshComponents[i]->GetTexId(), meshComponents[i]->HasTexture());
				vkCmdPushConstants(cmdBuffer, m_VkGraphicsPipeline->GetLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(PushConstant), &pc);

				vkCmdDrawIndexed(cmdBuffer, static_cast<uint32>(subMesh.indices.size()), 1, 0, 0, 0);
			}
		}

		vkCmdEndRenderPass(cmdBuffer);
		m_VkCommandBuffers->End(_imgIndex);
	}

	void VulkanRenderer::FetchMeshComponents() const
	{
		const auto& entities = EntityManager::GetAllEntities();
		for (const auto& entity : entities)
		{
			const auto& meshComponent = entity->GetComponent<MeshComponent>();
			if (meshComponent)
			{
				MeshSystem::Instance().AddMeshComponent(meshComponent);
			}
		}
	}
} // End of Banshee namespace