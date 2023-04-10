#include "VulkanFence.h"
#include "foundation/Logger.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanFence::VulkanFence(const VkDevice& _logicalDevice, const uint8 _count) :
		m_LogicalDevice(_logicalDevice),
		m_Fences{ VK_NULL_HANDLE }
	{
		BE_LOG(LogCategory::trace, "Vulkan fences creation stage: Start");

		VkFenceCreateInfo fenceCreateInfo{};
		fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;
		
		m_Fences.resize(_count);

		for (uint8 i = 0; i < _count; ++i)
		{
			if (vkCreateFence(_logicalDevice, &fenceCreateInfo, nullptr, &m_Fences[i]) != VK_SUCCESS)
			{
				BE_LOG(LogCategory::error, "Failed to create fence");
				throw std::runtime_error("ERROR: Failed to create fence\n");
			}
		}

		BE_LOG(LogCategory::info, "Vulkan fences created successfully");
		BE_LOG(LogCategory::trace, "Vulkan fences creation stage: End");
	}
	
	VulkanFence::~VulkanFence()
	{
		for (size_t i = 0; i < m_Fences.size(); ++i)
		{
			vkDestroyFence(m_LogicalDevice, m_Fences[i], nullptr);
			m_Fences[i] = VK_NULL_HANDLE;
		}

		BE_LOG(LogCategory::trace, "Vulkan fence destroyed");
	}

	void VulkanFence::Wait(const uint8 _fenceIndex) const
	{
		vkWaitForFences(m_LogicalDevice, 1, &m_Fences[_fenceIndex], VK_TRUE, UINT64_MAX);
	}

	void VulkanFence::Reset(const uint8 _fenceIndex) const
	{
		vkResetFences(m_LogicalDevice, 1, &m_Fences[_fenceIndex]);
	}
} // End of Banshee namespace