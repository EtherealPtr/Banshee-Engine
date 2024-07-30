#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkSemaphore_T* VkSemaphore;

namespace Banshee
{
	class VulkanSemaphore
	{
	public:
		VulkanSemaphore(const VkDevice& _logicalDevice, const uint16 _count = 1);
		~VulkanSemaphore();

		std::vector<std::pair<VkSemaphore, VkSemaphore>>& Get() noexcept { return m_Semaphores; }

	private:
		VkDevice m_LogicalDevice;
		std::vector<std::pair<VkSemaphore, VkSemaphore>> m_Semaphores;
	};
} // End of Banshee namespace