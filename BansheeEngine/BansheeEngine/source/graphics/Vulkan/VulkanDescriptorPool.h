#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;
typedef struct VkDescriptorPool_T* VkDescriptorPool;

namespace Banshee
{
	class VulkanDescriptorPool
	{
	public:
		VulkanDescriptorPool(const VkDevice& _logicalDevice, const VkDescriptorSetLayout& _layout, const uint32 _maxSets);
		~VulkanDescriptorPool();

		VkDescriptorPool Get() const { return m_DescriptorPool; }

	private:
		VkDevice m_LogicalDevice;
		VkDescriptorSetLayout m_DescriptorSetLayout;
		VkDescriptorPool m_DescriptorPool;
	};
} // End of Banshee namespace