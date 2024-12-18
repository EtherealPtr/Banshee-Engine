#pragma once

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
		void UpdateDescriptorSets(const uint8 _descriptorSetIndex);
		void StaticUpdateDescriptorSets() noexcept;
		void RecordRenderCommands(const uint8 _imgIndex);

	private:
		const Window& m_Window;
		VulkanInstance m_VkInstance;
		VulkanSurface m_VkSurface;
		VulkanDevice m_VkDevice;
		VulkanSwapchain m_VkSwapchain;
		VulkanDepthBuffer m_DepthBuffer;
		VulkanRenderPass m_VkRenderPass;
		VulkanCommandPool m_VkCommandPool;
		VulkanCommandBuffer m_VkCommandBuffers;
		VulkanFramebuffer m_VkFramebuffers;
		VulkanSemaphore m_VkSemaphores;
		VulkanFence m_VkInFlightFences;
		VulkanTextureSampler m_VkTextureSampler;
		VulkanTextureManager m_VkTextureManager;
		VulkanDescriptorSetLayout m_VkDescriptorSetLayout;
		VulkanDescriptorPool m_VkDescriptorPool;
		VulkanGraphicsPipelineManager m_VkGraphicsPipelineManager;
		std::vector<VulkanUniformBuffer> m_VPUniformBuffers;
		std::vector<VulkanUniformBuffer> m_MaterialUniformBuffers;
		std::vector<VulkanUniformBuffer> m_LightUniformBuffers;
		std::vector<VulkanDescriptorSet> m_DescriptorSets;
		Camera m_Camera;
		MeshSystem m_MeshSystem;
		LightSystem m_LightSystem;
		TransformationSystem m_TransformationSystem;
		uint8 m_CurrentFrameIndex;
		uint64 m_MaterialDynamicBufferMemAlignment;
		std::unique_ptr<Material, void(*)(Material*) noexcept> m_MaterialDynamicBufferMemBlock;
		std::vector<DescriptorSetWriteBufferProperties> m_DescriptorSetWriteBufferProperties;
		std::vector<DescriptorSetWriteTextureProperties> m_DescriptorSetWriteTextureProperties;
	};
} // End of Banshee namespace