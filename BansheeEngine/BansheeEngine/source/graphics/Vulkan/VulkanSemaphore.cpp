#include "VulkanSemaphore.h"
#include "foundation/Logger.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanSemaphore::VulkanSemaphore(const VkDevice& _logicalDevice, const uint8 _count) :
		m_LogicalDevice(_logicalDevice),
		m_Semaphores(_count, { VK_NULL_HANDLE, VK_NULL_HANDLE })
	{
		BE_LOG(LogCategory::trace, "Vulkan semaphores creation stage: Start");

		m_Semaphores.resize(_count);

		for (uint8 i = 0; i < m_Semaphores.size(); ++i)
		{
			VkSemaphoreCreateInfo semaphoreCreateInfo{};
			semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			if (vkCreateSemaphore(_logicalDevice, &semaphoreCreateInfo, nullptr, &m_Semaphores[i].first) != VK_SUCCESS ||
				vkCreateSemaphore(_logicalDevice, &semaphoreCreateInfo, nullptr, &m_Semaphores[i].second) != VK_SUCCESS)
			{
				BE_LOG(LogCategory::error, "Failed to create semaphores");
				throw std::runtime_error("ERROR: Failed to create semaphores\n");
			}
		}
		
		BE_LOG(LogCategory::info, "Vulkan semaphores created successfully");
		BE_LOG(LogCategory::trace, "Vulkan semaphores creation stage: End");
	}

	VulkanSemaphore::~VulkanSemaphore()
	{
		for (auto& semaphores : m_Semaphores)
		{
			vkDestroySemaphore(m_LogicalDevice, semaphores.first, nullptr);
			semaphores.first = VK_NULL_HANDLE;

			vkDestroySemaphore(m_LogicalDevice, semaphores.second, nullptr);
			semaphores.second = VK_NULL_HANDLE;
		}

		BE_LOG(LogCategory::trace, "Vulkan semaphores destroyed");
	}
} // End of Banshee namespace