#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;

namespace Banshee
{
	class VulkanDescriptorSetLayout
	{
	public:
		VulkanDescriptorSetLayout(const VkDevice& _logicalDevice);
		~VulkanDescriptorSetLayout();

		VkDescriptorSetLayout Get() const noexcept { return m_DescriptorSetLayout; }

		VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
		VulkanDescriptorSetLayout& operator=(const VulkanDescriptorSetLayout&) = delete;
		VulkanDescriptorSetLayout(VulkanDescriptorSetLayout&&) = delete;
		VulkanDescriptorSetLayout& operator=(VulkanDescriptorSetLayout&&) = delete;

	private:
		VkDevice m_LogicalDevice;
		VkDescriptorSetLayout m_DescriptorSetLayout;
	};
} // End of Banshee namespace