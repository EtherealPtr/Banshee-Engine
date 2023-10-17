#include "VulkanDescriptorSetLayout.h"
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <array>

namespace Banshee
{
	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const VkDevice& _logicalDevice, const uint32_t _shaderStage) :
		m_LogicalDevice(_logicalDevice),
		m_DescriptorSetLayout(VK_NULL_HANDLE)
	{
		// View-projection binding
		std::array<VkDescriptorSetLayoutBinding, 2> layoutBindings{};
		layoutBindings[0].binding = 0;
		layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		layoutBindings[0].descriptorCount = 1;
		layoutBindings[0].stageFlags = _shaderStage;
		layoutBindings[0].pImmutableSamplers = nullptr;

		// Color dynamic buffer
		layoutBindings[1].binding = 1;
		layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		layoutBindings[1].descriptorCount = 1;
		layoutBindings[1].stageFlags = _shaderStage;
		layoutBindings[1].pImmutableSamplers = nullptr;

		VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutCreateInfo.bindingCount = static_cast<uint32_t>(layoutBindings.size());
		layoutCreateInfo.pBindings = layoutBindings.data();

		if (vkCreateDescriptorSetLayout(_logicalDevice, &layoutCreateInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create descriptor set layout");
		}
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		if (m_DescriptorSetLayout == VK_NULL_HANDLE)
			return;

		vkDestroyDescriptorSetLayout(m_LogicalDevice, m_DescriptorSetLayout, nullptr);
		m_DescriptorSetLayout = VK_NULL_HANDLE;
	}
}
