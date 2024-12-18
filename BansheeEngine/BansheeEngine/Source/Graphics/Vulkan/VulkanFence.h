#pragma once

#include "Foundation/Platform.h"
#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkFence_T* VkFence;

namespace Banshee
{
	class VulkanFence
	{
	public:
		VulkanFence(const VkDevice& _logicalDevice, const uint16 _count = 1);
		~VulkanFence();

		void Wait(const uint16 _fenceIndex = 1) const noexcept;
		void Reset(const uint16 _fenceIndex = 1) const noexcept;
		std::vector<VkFence>& Get() noexcept { return m_Fences; }

		VulkanFence(const VulkanFence&) = delete;
		VulkanFence& operator=(const VulkanFence&) = delete;
		VulkanFence(VulkanFence&&) = delete;
		VulkanFence& operator=(VulkanFence&&) = delete;

	private:
		VkDevice m_LogicalDevice;
		std::vector<VkFence> m_Fences;
	};
} // End of Banshee namespace