#pragma once

#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorPool_T* VkDescriptorPool;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;
typedef struct VkDescriptorSet_T* VkDescriptorSet;
struct DescriptorSetWriteProperties;

namespace Banshee
{
	class VulkanDescriptorSet
	{
	public:
		VulkanDescriptorSet(const VkDevice& _logicalDevice, const VkDescriptorPool& _descriptorPool, const VkDescriptorSetLayout& _descriptorLayout);
		~VulkanDescriptorSet();

		void UpdateDescriptorSet(const std::vector<DescriptorSetWriteProperties>& _descriptorSetWriteProperties);
		VkDescriptorSet Get() const { return m_DescriptorSet; }

	private:
		VkDevice m_LogicalDevice;
		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSet m_DescriptorSet;
	};
}