#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkCommandPool_T* VkCommandPool;

namespace Banshee
{
	class VulkanCommandPool
	{
	public:
		VulkanCommandPool(const VkDevice& _logicalDevice, const uint32 _queueFamilyIndex);
		~VulkanCommandPool();

		VkCommandPool Get() const noexcept { return m_CommandPool; }

		VulkanCommandPool(const VulkanCommandPool&) = delete;
		VulkanCommandPool& operator=(const VulkanCommandPool&) = delete;
		VulkanCommandPool(VulkanCommandPool&&) = delete;
		VulkanCommandPool& operator=(VulkanCommandPool&&) = delete;

	private:
		VkDevice m_LogicalDevice;
		VkCommandPool m_CommandPool;
	};
} // End of Banshee namespace