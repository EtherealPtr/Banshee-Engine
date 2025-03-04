#include "VulkanDescriptorSet.h"
#include "VulkanDescriptorSetWriters.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan_core.h>

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

	void VulkanDescriptorSet::UpdateDescriptorSet(const std::vector<VulkanDescriptorSetBufferWriter>& _descriptorSetBufferWriters) const noexcept
	{
		for (const auto& writer : _descriptorSetBufferWriters)
		{
			VkDescriptorBufferInfo bufferInfo{};
			bufferInfo.offset = 0;
			bufferInfo.buffer = writer.m_Buffer;
			bufferInfo.range = writer.m_BufferRange;

			VkWriteDescriptorSet descriptorSetWriter{};
			descriptorSetWriter.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorSetWriter.dstSet = m_DescriptorSet;
			descriptorSetWriter.dstBinding = writer.m_Binding;
			descriptorSetWriter.descriptorCount = 1;
			descriptorSetWriter.descriptorType = writer.m_DescriptorType;
			descriptorSetWriter.pBufferInfo = &bufferInfo;
			descriptorSetWriter.pImageInfo = nullptr;

			vkUpdateDescriptorSets(m_LogicalDevice, 1, &descriptorSetWriter, 0, nullptr);
		}
	}

	void VulkanDescriptorSet::UpdateDescriptorSet(const std::vector<VulkanDescriptorSetTextureWriter>& _descriptorSetTextureWriters) const noexcept
	{
		for (const auto& writer : _descriptorSetTextureWriters)
		{
			std::vector<VkDescriptorImageInfo> imageInfos{};
			imageInfos.reserve(writer.m_ImageViews.size());

			VkDescriptorImageInfo imageInfo{};
			imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;

			if (writer.m_DescriptorType == VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER)
			{
				imageInfo.imageLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL;

				if (!writer.m_ImageViews.empty())
				{
					for (const auto& imageView : writer.m_ImageViews)
					{
						imageInfo.imageView = imageView;
						imageInfo.sampler = writer.m_Sampler;
						imageInfos.emplace_back(imageInfo);
					}
				}
				else
				{
					imageInfo.imageView = VK_NULL_HANDLE;
					imageInfo.sampler = writer.m_Sampler;
					imageInfos.emplace_back(imageInfo);
				}
			}
			else
			{
				if (!writer.m_ImageViews.empty())
				{
					for (const auto& imageView : writer.m_ImageViews)
					{
						imageInfo.imageView = imageView;
						imageInfo.sampler = VK_NULL_HANDLE;
						imageInfos.emplace_back(imageInfo);
					}
				}
				else
				{
					imageInfo.imageView = VK_NULL_HANDLE;
					imageInfo.sampler = writer.m_Sampler;
					imageInfos.emplace_back(imageInfo);
				}
			}

			VkWriteDescriptorSet descriptorSetWriter{};
			descriptorSetWriter.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorSetWriter.dstSet = m_DescriptorSet;
			descriptorSetWriter.dstBinding = writer.m_Binding;
			descriptorSetWriter.descriptorCount = static_cast<uint32>(imageInfos.size());
			descriptorSetWriter.descriptorType = writer.m_DescriptorType;
			descriptorSetWriter.pBufferInfo = nullptr;
			descriptorSetWriter.pImageInfo = imageInfos.data();

			vkUpdateDescriptorSets(m_LogicalDevice, 1, &descriptorSetWriter, 0, nullptr);
		}
	}
} // End of namespace