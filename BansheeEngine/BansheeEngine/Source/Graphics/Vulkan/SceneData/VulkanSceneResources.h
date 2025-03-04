#pragma once

#include "Graphics/Vulkan/VulkanDepthBuffer.h"
#include "Graphics/Vulkan/RenderPass/VulkanSceneRenderPass.h"
#include "Graphics/Vulkan/Layout/VulkanSceneDescriptorSetLayout.h"
#include "Graphics/Vulkan/Pipeline/VulkanGraphicsPipeline.h"
#include "Graphics/Vulkan/VulkanUniformBuffer.h"
#include "Graphics/Vulkan/VulkanDescriptorSet.h"
#include "Graphics/Vulkan/VulkanDescriptorSetWriters.h"
#include "Graphics/Components/Light/LightData.h"
#include <glm/glm.hpp>
#include <memory>
#include <span> 

typedef struct VkSampler_T* VkSampler;

namespace Banshee
{
	class VulkanRenderContext;
	class Material;
	class MeshSystem;
	struct ViewProjMatrix;

	class VulkanSceneResources
	{
	public:
		VulkanSceneResources(const VulkanRenderContext& _renderContext, const VkDescriptorPool& _descriptorPool, const uint32 _flags);

		void UpdateMeshUniformBuffers(const MeshSystem& _meshSystem);
		void UpdateVPUniformBuffers(ViewProjMatrix& _viewProj, const uint32 _imgIndex) const;
		void UpdateLightSpaceUniformBuffer(glm::mat4& _lightSpaceMatrix) const noexcept;
		void UpdateLightUniformBuffer(std::span<const LightData> _lightData, const uint32 _currentFrameIndex);
		void SetSceneTextures(const std::vector<VkImageView>& _textureViews, const VkSampler& _sampler);
		void SetSceneShadowMap(const VkImageView& _shadowMapView, const VkSampler& _sampler);
		void RecreateDepthBuffer(const uint32 _w, const uint32 _h, const uint32 _flags);
		void UpdateStaticDescriptorSets();
		void UpdateDescriptorSets(const uint32 _imgIndex);
		const VulkanDepthBuffer& GetDepthBuffer() const noexcept { return m_DepthBuffer; }
		const VkRenderPass& GetRenderPass() const noexcept { return m_RenderPass.Get(); }
		const VulkanSceneDescriptorSetLayout& GetDescriptorSetLayout() const noexcept { return m_DescriptorSetLayout; }
		const VulkanGraphicsPipeline& GetPipeline() const noexcept { return m_Pipeline; }
		std::vector<VulkanDescriptorSet>& GetDescriptorSets() noexcept { return m_DescriptorSets; }
		uint64 GetMaterialMemoryAlignment() const noexcept { return m_MaterialDynamicBufferMemAlignment; }

	private:
		void AllocateDynamicBufferSpace(const VulkanRenderContext& _renderContext) noexcept;
		void CreateDescriptorSetWriteBufferProperties();

	private:
		VulkanDepthBuffer m_DepthBuffer;
		VulkanSceneRenderPass m_RenderPass;
		VulkanSceneDescriptorSetLayout m_DescriptorSetLayout;
		VulkanGraphicsPipeline m_Pipeline;
		std::vector<VulkanDescriptorSetBufferWriter> m_DescriptorSetWriters;
		std::vector<VulkanDescriptorSetTextureWriter> m_DescriptorSetTextureWriters;
		uint64 m_MaterialDynamicBufferMemAlignment;
		std::unique_ptr<Material, void(*)(Material*) noexcept> m_MaterialDynamicBufferMemBlock;
		size_t m_NumOfSwapChainImages;
		std::vector<VulkanUniformBuffer> m_VPUniformBuffers;
		std::vector<VulkanUniformBuffer> m_MaterialUniformBuffers;
		std::vector<VulkanUniformBuffer> m_LightUniformBuffers;
		std::vector<VulkanDescriptorSet> m_DescriptorSets;
		VulkanUniformBuffer m_LightSpaceUniformBuffer;
	};
} // End of namespace