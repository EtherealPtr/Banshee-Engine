#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorPool_T* VkDescriptorPool;

namespace Banshee
{
	class VulkanDescriptorPool
	{
	public:
		VulkanDescriptorPool(const VkDevice& _logicalDevice, const uint32 _maxSets);
		~VulkanDescriptorPool();

		VkDescriptorPool Get() const noexcept { return m_DescriptorPool; }

		VulkanDescriptorPool(const VulkanDescriptorPool&) = delete;
		VulkanDescriptorPool& operator=(const VulkanDescriptorPool&) = delete;
		VulkanDescriptorPool(VulkanDescriptorPool&&) = delete;
		VulkanDescriptorPool& operator=(VulkanDescriptorPool&&) = delete;

	private:
		VkDevice m_LogicalDevice;
		VkDescriptorPool m_DescriptorPool;
	};
} // End of Banshee namespace