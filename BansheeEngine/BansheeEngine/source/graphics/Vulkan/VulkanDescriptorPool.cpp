#include "VulkanDescriptorPool.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <array>

namespace Banshee
{
	VulkanDescriptorPool::VulkanDescriptorPool(const VkDevice& _logicalDevice, const uint32 _maxSets) :
		m_LogicalDevice(_logicalDevice),
		m_DescriptorPool(VK_NULL_HANDLE)
	{
		BE_LOG(LogCategory::Trace, "[DESCRIPTOR POOL]: Creating descriptor pool");

		std::array<VkDescriptorPoolSize, 4> poolSizes{};
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = 1;
		BE_LOG(LogCategory::Trace, "[DESCRIPTOR POOL]: Added descriptor pool size of type VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER");

		poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		poolSizes[1].descriptorCount = 1;
		BE_LOG(LogCategory::Trace, "[DESCRIPTOR POOL]: Added descriptor pool size of type VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC");

		poolSizes[2].type = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		poolSizes[2].descriptorCount = 1;
		BE_LOG(LogCategory::Trace, "[DESCRIPTOR POOL]: Added descriptor pool size of type VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE");

		poolSizes[3].type = VK_DESCRIPTOR_TYPE_SAMPLER;
		poolSizes[3].descriptorCount = 1;
		BE_LOG(LogCategory::Trace, "[DESCRIPTOR POOL]: Added descriptor pool size of type VK_DESCRIPTOR_TYPE_SAMPLER");

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
			return;

		vkDestroyDescriptorPool(m_LogicalDevice, m_DescriptorPool, nullptr);
		m_DescriptorPool = VK_NULL_HANDLE;
	}
}
