#pragma once

#include <cstdint>

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;

namespace Banshee
{
	class VulkanDescriptorSetLayout
	{
	public:
		VulkanDescriptorSetLayout(const VkDevice& _logicalDevice, const uint32_t _shaderStage);
		~VulkanDescriptorSetLayout();

		VkDescriptorSetLayout Get() const { return m_DescriptorSetLayout; }

	private:
		VkDevice m_LogicalDevice;
		VkDescriptorSetLayout m_DescriptorSetLayout;
	};
}