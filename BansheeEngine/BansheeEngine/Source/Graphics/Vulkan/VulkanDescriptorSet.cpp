#include "VulkanDescriptorSet.h"
#include "VulkanDescriptorSetProperties.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanDescriptorSet::VulkanDescriptorSet(const VkDevice& _logicalDevice, const VkDescriptorPool& _descriptorPool, const VkDescriptorSetLayout& _descriptorLayout) :
		m_LogicalDevice{ _logicalDevice },
		m_DescriptorPool{ _descriptorPool },
		m_DescriptorSet{ VK_NULL_HANDLE }
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

	void VulkanDescriptorSet::UpdateDescriptorSet(const std::vector<DescriptorSetWriteBufferProperties>& _descriptorSetWriteBufProperties)
	{
		for (const auto& writeBufProperties : _descriptorSetWriteBufProperties)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.offset = 0;
			bufferInfo.buffer = writeBufProperties.m_Buffer;
			bufferInfo.range = writeBufProperties.m_BufferRange;

			VkWriteDescriptorSet descriptorSetWriter{};
			descriptorSetWriter.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorSetWriter.dstSet = m_DescriptorSet;
			descriptorSetWriter.dstBinding = writeBufProperties.m_Binding;
			descriptorSetWriter.descriptorCount = 1;
			descriptorSetWriter.descriptorType = writeBufProperties.m_DescriptorType;
			descriptorSetWriter.pBufferInfo = &bufferInfo;
			descriptorSetWriter.pImageInfo = nullptr;

			vkUpdateDescriptorSets(m_LogicalDevice, 1, &descriptorSetWriter, 0, nullptr);
		}
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(const std::vector<DescriptorSetWriteTextureProperties>& _descriptorSetWriteTexProperties)
	{
		for (const auto& writeTexProperties : _descriptorSetWriteTexProperties)
		{
			std::vector<VkDescriptorImageInfo> imageInfos{};
			uint32 descriptorCount = 1;

			if (writeTexProperties.m_ImageViews.size() > 0)
			{
				descriptorCount = static_cast<uint32>(writeTexProperties.m_ImageViews.size());

				for (uint32 i = 0; i < writeTexProperties.m_ImageViews.size(); ++i)
				{
					VkDescriptorImageInfo imageInfo{};
					imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
					imageInfo.imageView = writeTexProperties.m_ImageViews[i];
					imageInfo.sampler = VK_NULL_HANDLE;
					imageInfos.emplace_back(imageInfo);
				}
			}
			else
			{
				VkDescriptorImageInfo imageInfo{};
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
				imageInfo.imageView = VK_NULL_HANDLE;
				imageInfo.sampler = writeTexProperties.m_Sampler;
				imageInfos.emplace_back(imageInfo);
			}

			VkWriteDescriptorSet descriptorSetWriter{};
			descriptorSetWriter.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorSetWriter.dstSet = m_DescriptorSet;
			descriptorSetWriter.dstBinding = writeTexProperties.m_Binding;
			descriptorSetWriter.descriptorCount = descriptorCount;
			descriptorSetWriter.descriptorType = writeTexProperties.m_DescriptorType;
			descriptorSetWriter.pBufferInfo = nullptr;
			descriptorSetWriter.pImageInfo = imageInfos.data();

			vkUpdateDescriptorSets(m_LogicalDevice, 1, &descriptorSetWriter, 0, nullptr);
		}
	}
} // End of Banshee namespace