#include "VulkanRenderPassManager.h"

namespace Banshee
{
	VulkanRenderPassManager::VulkanRenderPassManager(const VkDevice& _device, const VkFormat _swapchainFormat, const VkFormat _sceneDepthFormat, const VkFormat _shadowDepthFormat)
	{
		m_RenderPasses.emplace(std::piecewise_construct,
			std::forward_as_tuple(SceneDataEnum::Scene),
			std::forward_as_tuple(_device, _swapchainFormat, _sceneDepthFormat));

		m_RenderPasses.emplace(std::piecewise_construct,
			std::forward_as_tuple(SceneDataEnum::DepthOnly),
			std::forward_as_tuple(_device, _shadowDepthFormat));
	}

	const VulkanRenderPass* const VulkanRenderPassManager::GetRenderPass(const SceneDataEnum _type) const
	{
		return &m_RenderPasses.at(_type);
	}
} // End of namespace