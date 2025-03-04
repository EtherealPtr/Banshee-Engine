#include "VulkanSceneShadowResources.h"
#include "Graphics/Vulkan/VulkanRenderContext.h"
#include "Graphics/Systems/LightSystem.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vulkan/vulkan_core.h>

namespace Banshee
{
	VulkanSceneShadowResources::VulkanSceneShadowResources(const VulkanRenderContext& _renderContext, const VkDescriptorPool& _descriptorPool, const uint32 _flags) :
		m_Width{ 1024 },
		m_Height{ 1024 },
		m_DepthBuffer{ _renderContext.GetDevice(), m_Width, m_Height, _flags },
		m_RenderPass{ _renderContext.GetDevice().GetLogicalDevice(), m_DepthBuffer.GetFormat() },
		m_DescriptorSetLayout{ _renderContext.GetDevice().GetLogicalDevice() },
		m_Pipeline{ _renderContext, m_DescriptorSetLayout.Get(), m_RenderPass.Get(), m_Width, m_Height },
		m_DescriptorSet{ _renderContext.GetDevice().GetLogicalDevice(), _descriptorPool, m_DescriptorSetLayout.Get() },
		m_UniformBuffer{ _renderContext.GetDevice().GetLogicalDevice(), _renderContext.GetDevice().GetPhysicalDevice(), sizeof(glm::mat4), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT },
		m_LightSpaceMatrix{ glm::mat4(1.0f) }
	{
		m_DescriptorSetWriters.Initialize(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER);
		m_DescriptorSetWriters.SetBuffer(m_UniformBuffer.GetBuffer(), m_UniformBuffer.GetBufferSize());
	}

	void VulkanSceneShadowResources::UpdateShadowUniformBuffer(const LightSystem& _lightSystem)
	{
		auto directionalLightComponent{ _lightSystem.GetDirectionalLight() };
		if (!directionalLightComponent.has_value())
		{
			return;
		}

		const glm::vec3 lightDir{ glm::normalize(directionalLightComponent->GetLightData().m_Direction) };
		const glm::vec3 lightPos{ -lightDir * 10.0f };
		const glm::mat4 lightViewMatrix{ glm::lookAt(lightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)) };
		const glm::mat4 lightProjMatrix{ glm::ortho(-20.0f, 20.0f, -20.0f, 20.0f, 0.1f, 50.0f) };
		m_LightSpaceMatrix = lightProjMatrix * lightViewMatrix;

		m_UniformBuffer.CopyData(&m_LightSpaceMatrix);
		m_DescriptorSet.UpdateDescriptorSet({ m_DescriptorSetWriters });
	}

	void VulkanSceneShadowResources::RecreateDepthBuffer(const uint32 _flags)
	{
		m_DepthBuffer.RecreateDepthBuffer(m_Width, m_Height, _flags);
	}
} // End of namespace