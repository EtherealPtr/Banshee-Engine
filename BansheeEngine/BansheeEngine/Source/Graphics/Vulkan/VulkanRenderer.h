#pragma once

#include "VulkanRenderContext.h"
#include "VulkanDepthBuffer.h"
#include "VulkanRenderPassManager.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSet.h"
#include "VulkanPipelineManager.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBuffer.h"
#include "VulkanFramebuffer.h"
#include "VulkanFrameResources.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanUniformBuffer.h"
#include "VulkanDescriptorSetProperties.h"
#include "VulkanTextureManager.h"
#include "VulkanTextureSampler.h"
#include "Graphics/Systems/MeshSystem.h"
#include "Graphics/Systems/LightSystem.h"
#include "Graphics/Systems/TransformationSystem.h"
#include "Graphics/Camera.h"

namespace Banshee
{
	class Window;
	class Material;

	class VulkanRenderer
	{
	public:
		explicit VulkanRenderer(const Window& _window);
		~VulkanRenderer();

		void DrawFrame(const double _deltaTime);

		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(const VulkanRenderer&) = delete;
		VulkanRenderer(VulkanRenderer&&) = delete;
		VulkanRenderer& operator=(VulkanRenderer&&) = delete;

	private:
		void InitializeGraphicsComponents();
		void AllocateDynamicBufferSpace() noexcept;
		void CreateDescriptorSetWriteBufferProperties();
		void UpdateMaterialData();
		void UpdateLightData();
		void UpdateShadowSceneDescriptorSets();
		void UpdateSceneDescriptorSets(const uint8 _descriptorSetIndex);
		void StaticUpdateDescriptorSets() noexcept;
		void RecordRenderCommands(const uint8 _imgIndex);
		void RenderShadowMap(const VkCommandBuffer& _cmdBuffer);
		void RenderScene(const VkCommandBuffer& _cmdBuffer, const uint8 _imgIndex);

	private:
		const Window& m_Window;
		VulkanRenderContext m_RenderContext;
		VulkanDepthBuffer m_VkSceneDepthBuffer;
		VulkanDepthBuffer m_VkShadowDepthBuffer;
		VulkanRenderPassManager m_RenderPassManager;
		VulkanCommandPool m_VkCommandPool;
		VulkanCommandBuffer m_VkCommandBuffers;
		VulkanFrameResources m_FrameResources;
		VulkanSemaphore m_VkSemaphores;
		VulkanFence m_VkInFlightFences;
		VulkanTextureSampler m_VkTextureSampler;
		VulkanTextureManager m_VkTextureManager;
		VulkanDescriptorSetLayout m_VkSceneDescriptorSetLayout;
		VulkanDescriptorSetLayout m_VkShadowDescriptorSetLayout;
		VulkanDescriptorPool m_VkDescriptorPool;
		VulkanPipelineManager m_PipelineManager;
		std::vector<VulkanUniformBuffer> m_VPUniformBuffers;
		std::vector<VulkanUniformBuffer> m_MaterialUniformBuffers;
		std::vector<VulkanUniformBuffer> m_LightUniformBuffers;
		std::vector<VulkanDescriptorSet> m_DescriptorSets;
		VulkanDescriptorSet m_VkShadowDescriptorSet;
		Camera m_Camera;
		MeshSystem m_MeshSystem;
		LightSystem m_LightSystem;
		TransformationSystem m_TransformationSystem;
		uint8 m_CurrentFrameIndex;
		uint64 m_MaterialDynamicBufferMemAlignment;
		std::unique_ptr<Material, void(*)(Material*) noexcept> m_MaterialDynamicBufferMemBlock;
		std::vector<DescriptorSetWriteBufferProperties> m_DescriptorSetWriteBufferProperties;
		std::vector<DescriptorSetWriteTextureProperties> m_DescriptorSetWriteTextureProperties;
		VulkanUniformBuffer m_ShadowUniformBuffer;
		DescriptorSetWriteBufferProperties m_ShadowDescriptorSetWriteBufferProperties;
		glm::mat4 m_LightSpaceMatrix;
	};
} // End of namespace