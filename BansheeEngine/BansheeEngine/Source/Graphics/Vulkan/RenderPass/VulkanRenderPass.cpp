#include "VulkanRenderPass.h"
#include <vulkan/vulkan_core.h>

namespace Banshee
{
	VulkanRenderPass::VulkanRenderPass(const VkDevice& _device) noexcept :
		m_Device{ _device },
		m_RenderPass{ VK_NULL_HANDLE }
	{}

	void VulkanRenderPass::CleanUp() noexcept
	{
		vkDestroyRenderPass(m_Device, m_RenderPass, nullptr);
		m_RenderPass = VK_NULL_HANDLE;
	}
} // End of namespace