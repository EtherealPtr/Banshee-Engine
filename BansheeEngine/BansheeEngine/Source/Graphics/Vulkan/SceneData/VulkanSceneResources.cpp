#include "VulkanSceneResources.h"
#include "Graphics/Vulkan/VulkanRenderContext.h"
#include "Graphics/DataStructs.h"
#include "Graphics/Material.h"
#include "Graphics/Systems/MeshSystem.h"
#include "Graphics/Systems/LightSystem.h"
#include "Graphics/MeshData.h"
#include <array>
#include <vulkan/vulkan_core.h>

namespace Banshee
{
	constexpr static uint64 g_MaxEntities{ 512 };

	VulkanSceneResources::VulkanSceneResources(const VulkanRenderContext& _renderContext, const VkDescriptorPool& _descriptorPool, const uint32 _flags) :
		m_DepthBuffer{ _renderContext.GetDevice(), _renderContext.GetSwapchain().GetWidth(), _renderContext.GetSwapchain().GetHeight(), _flags},
		m_RenderPass{ _renderContext.GetDevice().GetLogicalDevice(), _renderContext.GetSwapchain().GetFormat(), m_DepthBuffer.GetFormat()},
		m_DescriptorSetLayout{ _renderContext.GetDevice().GetLogicalDevice() },
		m_Pipeline{ _renderContext, m_DescriptorSetLayout.Get(), m_RenderPass.Get() },
		m_DescriptorSetWriters{ 4 },
		m_DescriptorSetTextureWriters{ 3 },
		m_MaterialDynamicBufferMemAlignment{ 0 },
		m_MaterialDynamicBufferMemBlock{ nullptr, [](Material* _ptr) noexcept { _aligned_free(_ptr); } },
		m_NumOfSwapChainImages{ _renderContext.GetSwapchain().GetImageViews().size() },
		m_LightSpaceUniformBuffer{ _renderContext.GetDevice().GetLogicalDevice(), _renderContext.GetDevice().GetPhysicalDevice(), sizeof(glm::mat4), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT }
	{
		AllocateDynamicBufferSpace(_renderContext);
		CreateDescriptorSetWriteBufferProperties();

		m_VPUniformBuffers.reserve(m_NumOfSwapChainImages);
		m_MaterialUniformBuffers.reserve(m_NumOfSwapChainImages);
		m_LightUniformBuffers.reserve(m_NumOfSwapChainImages);
		m_DescriptorSets.reserve(m_NumOfSwapChainImages);

		for (size_t i = 0; i < m_NumOfSwapChainImages; ++i)
		{
			m_VPUniformBuffers.emplace_back(_renderContext.GetDevice().GetLogicalDevice(), _renderContext.GetDevice().GetPhysicalDevice(), sizeof(ViewProjMatrix), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			m_MaterialUniformBuffers.emplace_back(_renderContext.GetDevice().GetLogicalDevice(), _renderContext.GetDevice().GetPhysicalDevice(), m_MaterialDynamicBufferMemAlignment * g_MaxEntities, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);
			m_LightUniformBuffers.emplace_back(_renderContext.GetDevice().GetLogicalDevice(), _renderContext.GetDevice().GetPhysicalDevice(), sizeof(LightBuffer), VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
			m_DescriptorSets.emplace_back(_renderContext.GetDevice().GetLogicalDevice(), _descriptorPool, m_DescriptorSetLayout.Get());

			m_DescriptorSetWriters.at(0).SetBuffer(m_VPUniformBuffers.at(i).GetBuffer(), m_VPUniformBuffers.at(i).GetBufferSize());
			m_DescriptorSetWriters.at(1).SetBuffer(m_MaterialUniformBuffers.at(i).GetBuffer(), m_MaterialDynamicBufferMemAlignment);
			m_DescriptorSetWriters.at(2).SetBuffer(m_LightUniformBuffers.at(i).GetBuffer(), m_LightUniformBuffers.at(i).GetBufferSize());
			m_DescriptorSetWriters.at(3).SetBuffer(m_LightSpaceUniformBuffer.GetBuffer(), m_LightSpaceUniformBuffer.GetBufferSize());
		}
	}

	void VulkanSceneResources::AllocateDynamicBufferSpace(const VulkanRenderContext& _renderContext) noexcept
	{
		const VkDeviceSize minUniformBufferOffset{ _renderContext.GetDevice().GetLimits().minUniformBufferOffsetAlignment };
		m_MaterialDynamicBufferMemAlignment = (sizeof(Material) + minUniformBufferOffset - 1) & ~(_renderContext.GetDevice().GetLimits().minUniformBufferOffsetAlignment - 1);
		m_MaterialDynamicBufferMemBlock.reset(static_cast<Material*>(_aligned_malloc(m_MaterialDynamicBufferMemAlignment * g_MaxEntities, m_MaterialDynamicBufferMemAlignment)));
	}

	void VulkanSceneResources::CreateDescriptorSetWriteBufferProperties()
	{
		m_DescriptorSetWriters.at(0).Initialize(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		m_DescriptorSetWriters.at(1).Initialize(1, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC);
		m_DescriptorSetWriters.at(2).Initialize(4, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER);
		m_DescriptorSetWriters.at(3).Initialize(5, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);

		m_DescriptorSetTextureWriters.at(0).Initialize(2, VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE);
		m_DescriptorSetTextureWriters.at(1).Initialize(3, VK_DESCRIPTOR_TYPE_SAMPLER);
		m_DescriptorSetTextureWriters.at(2).Initialize(6, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER);
	}

	void VulkanSceneResources::UpdateDescriptorSets(const uint32 _imgIndex)
	{
		m_DescriptorSets.at(_imgIndex).UpdateDescriptorSet(m_DescriptorSetWriters);
	}

	void VulkanSceneResources::UpdateMeshUniformBuffers(const MeshSystem& _meshSystem)
	{
		for (const auto& subMesh : _meshSystem.GetAllSubMeshes())
		{
			Material* materialData{ (Material*)((uint64)m_MaterialDynamicBufferMemBlock.get() + (subMesh.GetMeshId() * m_MaterialDynamicBufferMemAlignment)) };
			*materialData = { subMesh.GetMaterial().GetDiffuseColor(), subMesh.GetMaterial().GetSpecularColor()};

			for (size_t i = 0; i < m_DescriptorSets.size(); ++i)
			{
				m_MaterialUniformBuffers.at(i).CopyData(m_MaterialDynamicBufferMemBlock.get());
			}
		}
	}

	void VulkanSceneResources::UpdateVPUniformBuffers(ViewProjMatrix& _viewProj, const uint32 _imgIndex) const
	{
		m_VPUniformBuffers.at(_imgIndex).CopyData(&_viewProj);
	}

	void VulkanSceneResources::UpdateLightSpaceUniformBuffer(glm::mat4& _lightSpaceMatrix) const noexcept
	{
		m_LightSpaceUniformBuffer.CopyData(&_lightSpaceMatrix);
	}

	void VulkanSceneResources::UpdateLightUniformBuffer(std::span<const LightData> _lightData, const uint32 _currentFrameIndex)
	{
		LightBuffer lightBuffer{};
		lightBuffer.m_TotalLights = static_cast<uint8>(_lightData.size());

		std::copy(_lightData.begin(), _lightData.end(), lightBuffer.m_Lights);
		m_LightUniformBuffers.at(_currentFrameIndex).CopyData(&lightBuffer);
	}

	void VulkanSceneResources::SetSceneTextures(const std::vector<VkImageView>& _textureViews, const VkSampler& _sampler)
	{
		m_DescriptorSetTextureWriters.at(0).SetImageView(_textureViews);
		m_DescriptorSetTextureWriters.at(1).SetSampler(_sampler);
	}

	void VulkanSceneResources::SetSceneShadowMap(const VkImageView& _shadowMapView, const VkSampler& _sampler)
	{
		m_DescriptorSetTextureWriters.at(2).SetImageView({ _shadowMapView });
		m_DescriptorSetTextureWriters.at(2).SetSampler(_sampler);
	}

	void VulkanSceneResources::RecreateDepthBuffer(const uint32 _w, const uint32 _h, const uint32 _flags)
	{
		m_DepthBuffer.RecreateDepthBuffer(_w, _h, _flags);
	}

	void VulkanSceneResources::UpdateStaticDescriptorSets()
	{
		for (size_t i = 0; i < m_DescriptorSets.size(); ++i)
		{
			m_DescriptorSets.at(i).UpdateDescriptorSet(m_DescriptorSetTextureWriters);
		}
	}
} // End of namespace