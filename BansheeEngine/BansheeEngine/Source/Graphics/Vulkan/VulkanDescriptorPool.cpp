#include "VulkanDescriptorPool.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan.h>
#include <array>

namespace Banshee
{
	VulkanDescriptorPool::VulkanDescriptorPool(const VkDevice& _logicalDevice, const uint32 _maxSets) :
		m_LogicalDevice{ _logicalDevice },
		m_DescriptorPool{ VK_NULL_HANDLE }
	{
		BE_LOG(LogCategory::Trace, "[DESCRIPTOR POOL]: Creating descriptor pool");

		std::array<VkDescriptorPoolSize, 6> poolSizes =
		{
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 2 },
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1 },
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1 },
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_SAMPLER, 1 },
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1 },
			VkDescriptorPoolSize{ VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1 }
		};

		VkDescriptorPoolCreateInfo descriptorPoolCreateInfo{};
		descriptorPoolCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		descriptorPoolCreateInfo.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
		descriptorPoolCreateInfo.maxSets = _maxSets;
		descriptorPoolCreateInfo.poolSizeCount = static_cast<uint32>(poolSizes.size());
		descriptorPoolCreateInfo.pPoolSizes = poolSizes.data();

		if (vkCreateDescriptorPool(_logicalDevice, &descriptorPoolCreateInfo, nullptr, &m_DescriptorPool) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a descriptor pool");
		}

		BE_LOG(LogCategory::Info, "[DESCRIPTOR POOL]: Created descriptor pool");
	}

	VulkanDescriptorPool::~VulkanDescriptorPool()
	{
		if (m_DescriptorPool == VK_NULL_HANDLE)
		{
			return;
		}

		vkDestroyDescriptorPool(m_LogicalDevice, m_DescriptorPool, nullptr);
		m_DescriptorPool = VK_NULL_HANDLE;
	}
} // End of Banshee namespace