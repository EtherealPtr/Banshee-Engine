#include "VulkanSemaphore.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanSemaphore::VulkanSemaphore(const VkDevice& _logicalDevice, const uint16 _count) :
		m_LogicalDevice{ _logicalDevice },
		m_Semaphores{ _count, { VK_NULL_HANDLE, VK_NULL_HANDLE } }
	{
		BE_LOG(LogCategory::Trace, "[SEMAPHORE]: Creating %d semaphores", _count);

		m_Semaphores.resize(_count);

		for (size_t i = 0; i < m_Semaphores.size(); ++i)
		{
			VkSemaphoreCreateInfo semaphoreCreateInfo{};
			semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

			if (vkCreateSemaphore(_logicalDevice, &semaphoreCreateInfo, nullptr, &m_Semaphores[i].first) != VK_SUCCESS ||
				vkCreateSemaphore(_logicalDevice, &semaphoreCreateInfo, nullptr, &m_Semaphores[i].second) != VK_SUCCESS)
			{
				throw std::runtime_error("ERROR: Failed to create semaphores\n");
			}
		}

		BE_LOG(LogCategory::Info, "[SEMAPHORE]: Created semaphores");
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
	}
} // End of Banshee namespace