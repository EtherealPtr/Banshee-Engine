#include "VulkanDescriptorSet.h"
#include "VulkanDescriptorSetProperties.h"
#include "Foundation/Logging/Logger.h"
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
			BE_LOG(LogCategory::Warning, "[DESCRIPTOR SET]: Failed to allocate descriptor sets");
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
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.offset = 0;
			bufferInfo.buffer = writeProperties.buffer;
			bufferInfo.range = writeProperties.bufferRange;

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
			imageInfo.imageView = writeProperties.imageView;
			imageInfo.sampler = writeProperties.sampler;

			VkWriteDescriptorSet descriptorSetWriter{};
			descriptorSetWriter.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorSetWriter.dstSet = m_DescriptorSet;
			descriptorSetWriter.dstBinding = writeProperties.binding;
			descriptorSetWriter.descriptorCount = 1;
			descriptorSetWriter.descriptorType = writeProperties.descriptorType;
			descriptorSetWriter.pBufferInfo = &bufferInfo;
			descriptorSetWriter.pImageInfo = imageInfo.imageView != VK_NULL_HANDLE ? &imageInfo : nullptr;

			vkUpdateDescriptorSets(m_LogicalDevice, 1, &descriptorSetWriter, 0, nullptr);
		}
	}
}