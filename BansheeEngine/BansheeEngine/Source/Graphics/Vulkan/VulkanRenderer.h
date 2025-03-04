#pragma once

#include "VulkanRenderContext.h"
#include "VulkanDescriptorPool.h"
#include "SceneData/VulkanSceneResources.h"
#include "SceneData/VulkanSceneShadowResources.h"
#include "VulkanFramebuffer.h"
#include "VulkanCommandPool.h"
#include "VulkanCommandBuffer.h"
#include "VulkanSemaphore.h"
#include "VulkanFence.h"
#include "VulkanTextureSampler.h"
#include "VulkanTexture.h"
#include "VulkanVertexBufferManager.h"
#include "Graphics/Camera.h"
#include "Graphics/Systems/MeshSystem.h"
#include "Graphics/Systems/LightSystem.h"
#include "Graphics/Systems/TransformationSystem.h"

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
		void UpdateShadowSceneDescriptorSets();
		void RecordRenderCommands(const uint32 _imgIndex);
		void RenderShadowMap(const VkCommandBuffer& _cmdBuffer);
		void RenderScene(const VkCommandBuffer& _cmdBuffer, const uint32 _imgIndex);

	private:
		VulkanRenderContext m_RenderContext;
		VulkanDescriptorPool m_DescriptorPool;
		VulkanSceneResources m_SceneResources;
		VulkanSceneShadowResources m_ShadowSceneResources;
		VulkanFramebuffer m_FramebufferShadow;
		VulkanCommandPool m_CommandPool;
		VulkanCommandBuffer m_CommandBuffers;
		VulkanSemaphore m_Semaphore;
		VulkanFence m_InFlightFences;
		std::vector<VulkanFramebuffer> m_Framebuffers;
		VulkanTextureSampler m_TextureSampler;
		VulkanTexture m_Textures;
		VulkanVertexBufferManager m_VertexBufferManager;
		Camera m_Camera;
		MeshSystem m_MeshSystem;
		LightSystem m_LightSystem;
		TransformationSystem m_TransformationSystem;
		uint32 m_CurrentFrameIndex;
	};
} // End of namespace