#include "VulkanDescriptorSet.h"
#include "VulkanDescriptorSetProperties.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanDescriptorSet::VulkanDescriptorSet(const VkDevice& _logicalDevice, const VkDescriptorPool& _descriptorPool, const VkDescriptorSetLayout& _descriptorLayout) :
		m_LogicalDevice(_logicalDevice),
		m_DescriptorPool(_descriptorPool),
		m_DescriptorSet(VK_NULL_HANDLE)
	{
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = _descriptorPool;
		allocInfo.descriptorSetCount = 1;
		allocInfo.pSetLayouts = &_descriptorLayout;

		if (vkAllocateDescriptorSets(_logicalDevice, &allocInfo, &m_DescriptorSet) != VK_SUCCESS)
		{
			printf("WARNING: Failed to allocate descriptor sets\n");
		}
	}

	VulkanDescriptorSet::~VulkanDescriptorSet()
	{
		vkFreeDescriptorSets(m_LogicalDevice, m_DescriptorPool, 1, &m_DescriptorSet);
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(const std::vector<DescriptorSetWriteProperties>& _descriptorSetWriteProperties)
	{
		for (const auto& writeProperties : _descriptorSetWriteProperties)
		{
			VkDescriptorBufferInfo bufferInfo;
			bufferInfo.offset = 0;
			bufferInfo.buffer = writeProperties.buffer;
			bufferInfo.range = writeProperties.bufferRange;

			VkWriteDescriptorSet descriptorSetWriter{};
			descriptorSetWriter.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorSetWriter.dstSet = m_DescriptorSet;
			descriptorSetWriter.dstBinding = writeProperties.binding;
			descriptorSetWriter.descriptorCount = 1;
			descriptorSetWriter.descriptorType = writeProperties.descriptorType;
			descriptorSetWriter.pBufferInfo = &bufferInfo;
			descriptorSetWriter.pImageInfo = nullptr;

			vkUpdateDescriptorSets(m_LogicalDevice, 1, &descriptorSetWriter, 0, nullptr);
		}
	}
}
