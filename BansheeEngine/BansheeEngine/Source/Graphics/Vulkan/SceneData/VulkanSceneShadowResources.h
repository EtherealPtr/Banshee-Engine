#pragma once

#include "Graphics/Vulkan/VulkanDepthBuffer.h"
#include "Graphics/Vulkan/RenderPass/VulkanShadowRenderPass.h"
#include "Graphics/Vulkan/Layout/VulkanShadowSceneDiscriptorSetLayout.h"
#include "Graphics/Vulkan/Pipeline/VulkanShadowPipeline.h"
#include "Graphics/Vulkan/VulkanUniformBuffer.h"
#include "Graphics/Vulkan/VulkanDescriptorSet.h"
#include "Graphics/Vulkan/VulkanDescriptorSetWriters.h"
#include <glm/glm.hpp>

namespace Banshee
{
	class VulkanRenderContext;
	class LightSystem;

	class VulkanSceneShadowResources
	{
	public:
		VulkanSceneShadowResources(const VulkanRenderContext& _renderContext, const VkDescriptorPool& _descriptorPool, const uint32 _flags);

		uint32 GetShadowWidth() const noexcept { return m_Width; }
		uint32 GetShadowHeight() const noexcept { return m_Height; }
		void UpdateShadowUniformBuffer(const LightSystem& _lightSystem);
		void RecreateDepthBuffer(const uint32 _flags);
		const VulkanDepthBuffer& GetDepthBuffer() const noexcept { return m_DepthBuffer; }
		const VkRenderPass& GetRenderPass() const noexcept { return m_RenderPass.Get(); }
		const VulkanShadowSceneDiscriptorSetLayout& GetDescriptorSetLayout() const noexcept { return m_DescriptorSetLayout; }
		const VulkanShadowPipeline& GetPipeline() const noexcept { return m_Pipeline; }
		VulkanDescriptorSet& GetDescriptorSetShadow() noexcept { return m_DescriptorSet; }
		const glm::mat4& GetLightSpaceMatrix() const noexcept { return m_LightSpaceMatrix; }

	private:
		uint32 m_Width;
		uint32 m_Height;
		VulkanDepthBuffer m_DepthBuffer;
		VulkanShadowRenderPass m_RenderPass;
		VulkanShadowSceneDiscriptorSetLayout m_DescriptorSetLayout;
		VulkanShadowPipeline m_Pipeline;
		VulkanDescriptorSet m_DescriptorSet;
		VulkanUniformBuffer m_UniformBuffer;
		VulkanDescriptorSetBufferWriter m_DescriptorSetWriters;
		glm::mat4 m_LightSpaceMatrix;
	};
} // End of namespace