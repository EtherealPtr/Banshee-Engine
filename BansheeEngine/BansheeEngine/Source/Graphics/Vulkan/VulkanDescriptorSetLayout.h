#pragma once

#include "Graphics/SceneDataEnum.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;

namespace Banshee
{

	class VulkanDescriptorSetLayout
	{
	public:
		VulkanDescriptorSetLayout(const VkDevice& _logicalDevice, const SceneDataEnum _descriptorSetLayoutType);
		~VulkanDescriptorSetLayout();

		const VkDescriptorSetLayout& Get() noexcept { return m_DescriptorSetLayout; }

		VulkanDescriptorSetLayout(const VulkanDescriptorSetLayout&) = delete;
		VulkanDescriptorSetLayout& operator=(const VulkanDescriptorSetLayout&) = delete;
		VulkanDescriptorSetLayout(VulkanDescriptorSetLayout&&) = delete;
		VulkanDescriptorSetLayout& operator=(VulkanDescriptorSetLayout&&) = delete;

	private:
		VkDevice m_LogicalDevice;
		VkDescriptorSetLayout m_DescriptorSetLayout;
	};
} // End of namespace