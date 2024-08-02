#pragma once

#include "Foundation/Platform.h"
#include <vector>
#include <memory>

namespace Banshee
{
	class VulkanInstance;
	class VulkanSurface;
	class VulkanDevice;
	class VulkanSwapchain;
	class VulkanDepthBuffer;
	class VulkanRenderPass;
	class VulkanDescriptorSetLayout;
	class VulkanDescriptorPool;
	class VulkanDescriptorSet;
	class VulkanGraphicsPipelineManager;
	class VulkanCommandPool;
	class VulkanCommandBuffer;
	class VulkanFramebuffer;
	class VulkanSemaphore;
	class VulkanFence;
	class VulkanVertexBufferManager;
	class VulkanUniformBuffer;
	class VulkanTextureManager;
	class VulkanTextureSampler;
	class Window;
	class Camera;
	class Material;
	class MeshSystem;
	class LightSystem;

	class VulkanRenderer
	{
	public:
		explicit VulkanRenderer(const std::unique_ptr<Window>& _window);
		~VulkanRenderer();

		void DrawFrame(const double _deltaTime);

		VulkanRenderer(const VulkanRenderer&) = delete;
		VulkanRenderer& operator=(const VulkanRenderer&) = delete;
		VulkanRenderer(VulkanRenderer&&) = delete;
		VulkanRenderer& operator=(VulkanRenderer&&) = delete;

	private:
		void AllocateDynamicBufferSpace() noexcept;
		void UpdateMaterialData();
		void UpdateLightData();
		void UpdateDescriptorSets(const uint8 _descriptorSetIndex);
		void RecordRenderCommands(const uint8 _imgIndex);
		void FetchGraphicsComponents() const;

	private:
		std::unique_ptr<VulkanInstance> m_VkInstance;
		std::unique_ptr<VulkanSurface> m_VkSurface;
		std::unique_ptr<VulkanDevice> m_VkDevice;
		std::unique_ptr<VulkanSwapchain> m_VkSwapchain;
		std::unique_ptr<VulkanDepthBuffer> m_DepthBuffer;
		std::unique_ptr<VulkanRenderPass> m_VkRenderPass;
		std::unique_ptr<VulkanCommandPool> m_VkCommandPool;
		std::unique_ptr<VulkanCommandBuffer> m_VkCommandBuffers;
		std::unique_ptr<VulkanFramebuffer> m_VkFramebuffers;
		std::unique_ptr<VulkanSemaphore> m_VkSemaphores;
		std::unique_ptr<VulkanFence> m_VkInFlightFences;
		std::unique_ptr<VulkanVertexBufferManager> m_VertexBufferManager;
		std::unique_ptr<VulkanTextureSampler> m_VkTextureSampler;
		std::unique_ptr<VulkanTextureManager> m_VkTextureManager;
		std::unique_ptr<VulkanDescriptorSetLayout> m_VkDescriptorSetLayout;
		std::unique_ptr<VulkanDescriptorPool> m_VkDescriptorPool;
		std::unique_ptr<VulkanGraphicsPipelineManager> m_VkGraphicsPipelineManager;
		std::vector<std::unique_ptr<VulkanUniformBuffer>> m_VPUniformBuffers;
		std::vector<std::unique_ptr<VulkanUniformBuffer>> m_MaterialUniformBuffers;
		std::vector<std::unique_ptr<VulkanUniformBuffer>> m_LightUniformBuffers;
		std::vector<std::unique_ptr<VulkanDescriptorSet>> m_DescriptorSets;
		std::unique_ptr<Camera> m_Camera;
		uint8 m_CurrentFrameIndex;
		size_t m_MaterialDynamicBufferMemAlignment;
		std::unique_ptr<Material, void(*)(Material*) noexcept> m_MaterialDynamicBufferMemBlock;
		std::unique_ptr<MeshSystem> m_MeshSystem;
		std::unique_ptr<LightSystem> m_LightSystem;
	};
} // End of Banshee namespace