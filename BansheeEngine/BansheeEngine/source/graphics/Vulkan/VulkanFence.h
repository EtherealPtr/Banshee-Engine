#pragma once

#include "foundation/Platform.h"
#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkFence_T* VkFence;

namespace Banshee
{
	class VulkanFence
	{
	public:
		VulkanFence(const VkDevice& _logicalDevice, const uint8 _count = 1);
		~VulkanFence();

		void Wait(const uint8 _fenceIndex = 1) const;
		void Reset(const uint8 _fenceIndex = 1) const;
		std::vector<VkFence>& Get() { return m_Fences; }

	private:
		VkDevice m_LogicalDevice;
		std::vector<VkFence> m_Fences;
	};
} // End of Banshee namespace
