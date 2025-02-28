#pragma once

#include "VulkanRenderContext.h"
#include "VulkanDepthBuffer.h"
#include "VulkanRenderPassManager.h"
#include "VulkanDescriptorSetLayout.h"
#include "VulkanDescriptorPool.h"
#include "VulkanDescriptorSet.h"
#include "VulkanPipelineManager.h"
#include "VulkanCommandPool.h"
#include "VulkanFrameResources.h"
#include "VulkanUniformBuffer.h"
#include "VulkanDescriptorSetProperties.h"
#include "VulkanTexture.h"
#include "VulkanTextureSampler.h"
#include "Graphics/Systems/MeshSystem.h"
#include "Graphics/Systems/LightSystem.h"
#include "Graphics/Systems/TransformationSystem.h"
#include "Graphics/Camera.h"

namespace Banshee
{
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
		void UpdateSceneDescriptorSets(const uint32 _descriptorSetIndex);
		void StaticUpdateDescriptorSets() noexcept;
		void RecordRenderCommands(const uint32 _imgIndex);
		void RenderShadowMap(const VkCommandBuffer& _cmdBuffer);
		void RenderScene(const VkCommandBuffer& _cmdBuffer, const uint32 _imgIndex);

	private:
		VulkanRenderContext m_RenderContext;
		VulkanDepthBuffer m_DepthBufferScene;
		VulkanDepthBuffer m_DepthBufferShadow;
		VulkanRenderPassManager m_RenderPassManager;
		VulkanFrameResources m_FrameResources;
		VulkanTextureSampler m_TextureSampler;
		VulkanTexture m_Textures;
		VulkanDescriptorSetLayout m_DescriptorSetLayoutScene;
		VulkanDescriptorSetLayout m_DescriptorSetLayoutShadow;
		VulkanPipelineManager m_PipelineManager;
		VulkanDescriptorPool m_DescriptorPool;
		VulkanDescriptorSet m_DescriptorSetShadow;
		Camera m_Camera;
		MeshSystem m_MeshSystem;
		uint32 m_CurrentFrameIndex;
		uint64 m_MaterialDynamicBufferMemAlignment;
		std::unique_ptr<class Material, void(*)(Material*) noexcept> m_MaterialDynamicBufferMemBlock;
		VulkanUniformBuffer m_ShadowUniformBuffer;
		std::vector<VulkanUniformBuffer> m_VPUniformBuffers;
		std::vector<VulkanUniformBuffer> m_MaterialUniformBuffers;
		std::vector<VulkanUniformBuffer> m_LightUniformBuffers;
		std::vector<VulkanDescriptorSet> m_DescriptorSets;
		LightSystem m_LightSystem;
		TransformationSystem m_TransformationSystem;
		std::vector<DescriptorSetWriteBufferProperties> m_DescriptorSetWriteBufferProperties;
		std::vector<DescriptorSetWriteTextureProperties> m_DescriptorSetWriteTextureProperties;
		DescriptorSetWriteBufferProperties m_ShadowDescriptorSetWriteBufferProperties;
	};
} // End of namespace