#pragma once

#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkDescriptorPool_T* VkDescriptorPool;
typedef struct VkDescriptorSetLayout_T* VkDescriptorSetLayout;
typedef struct VkDescriptorSet_T* VkDescriptorSet;

namespace Banshee
{
	struct DescriptorSetWriteBufferProperties;
	struct DescriptorSetWriteTextureProperties;

	class VulkanDescriptorSet
	{
	public:
		VulkanDescriptorSet(const VkDevice& _logicalDevice, const VkDescriptorPool& _descriptorPool, const VkDescriptorSetLayout& _descriptorLayout);
		~VulkanDescriptorSet();

		void UpdateDescriptorSet(const std::vector<DescriptorSetWriteBufferProperties>& _descriptorSetWriteBufProperties);
		void UpdateDescriptorSet(const std::vector<DescriptorSetWriteTextureProperties>& _descriptorSetWriteTexProperties);
		VkDescriptorSet Get() const noexcept { return m_DescriptorSet; }

		VulkanDescriptorSet(const VulkanDescriptorSet&) = delete;
		VulkanDescriptorSet& operator=(const VulkanDescriptorSet&) = delete;
		VulkanDescriptorSet(VulkanDescriptorSet&&) = delete;
		VulkanDescriptorSet& operator=(VulkanDescriptorSet&&) = delete;

	private:
		VkDevice m_LogicalDevice;
		VkDescriptorPool m_DescriptorPool;
		VkDescriptorSet m_DescriptorSet;
	};
} // End of Banshee namespace