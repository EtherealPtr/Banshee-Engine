#include "VulkanCommandPool.h"
#include "foundation/Logger.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanCommandPool::VulkanCommandPool(const VkDevice& _logicalDevice, const uint32 _queueFamilyIndex) :
		m_LogicalDevice(_logicalDevice),
		m_CommandPool(VK_NULL_HANDLE)
	{
		BE_LOG(LogCategory::trace, "Vulkan command pool creation stage: Start");

		VkCommandPoolCreateInfo poolCreateInfo{};
		poolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolCreateInfo.queueFamilyIndex = _queueFamilyIndex;

		if (vkCreateCommandPool(_logicalDevice, &poolCreateInfo, nullptr, &m_CommandPool) != VK_SUCCESS)
		{
			BE_LOG(LogCategory::error, "Failed to create a command pool");
			throw std::runtime_error("ERROR: Failed to create a command pool");
		}

		BE_LOG(LogCategory::info, "Vulkan command pool created successfully");
		BE_LOG(LogCategory::trace, "Vulkan command pool creation stage: End");
	}
	
	VulkanCommandPool::~VulkanCommandPool()
	{
		vkDestroyCommandPool(m_LogicalDevice, m_CommandPool, nullptr);
		m_CommandPool = VK_NULL_HANDLE;
		BE_LOG(LogCategory::trace, "Vulkan command pool destroyed")
	}
} // End of Banshee namespace