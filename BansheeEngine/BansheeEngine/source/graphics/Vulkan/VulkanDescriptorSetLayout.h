#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;

namespace Banshee
{
	class VulkanDescriptorSetLayout
	{
	public:
		VulkanDescriptorSetLayout(const VkDevice& _logicalDevice, const uint32 _shaderStage);
		~VulkanDescriptorSetLayout();

		VkDescriptorSetLayout Get() const noexcept { return m_DescriptorSetLayout; }

	private:
		VkDevice m_LogicalDevice;
		VkDescriptorSetLayout m_DescriptorSetLayout;
	};
} // End of Banshee namespace