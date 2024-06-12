#include "VulkanDescriptorSetLayout.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/ResourceManager/Image/ImageManager.h"
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <array>

namespace Banshee
{
	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const VkDevice& _logicalDevice, const uint32 _shaderStage) :
		m_LogicalDevice(_logicalDevice),
		m_DescriptorSetLayout(VK_NULL_HANDLE)
	{
		BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Creating descriptor set layout");

		// View-projection binding
		std::array<VkDescriptorSetLayoutBinding, 4> layoutBindings{};
		layoutBindings[0].binding = 0;
		layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		layoutBindings[0].descriptorCount = 1;
		layoutBindings[0].stageFlags = _shaderStage;
		layoutBindings[0].pImmutableSamplers = nullptr;
		BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Added descriptor of type VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER at binding 0");

		// Color dynamic buffer
		layoutBindings[1].binding = 1;
		layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
		layoutBindings[1].descriptorCount = 1;
		layoutBindings[1].stageFlags = _shaderStage;
		layoutBindings[1].pImmutableSamplers = nullptr;
		BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Added descriptor of type VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC at binding 1");

		// Textures
		const uint32 MAX_TEXTURES = 16;
		layoutBindings[2].binding = 2;
		layoutBindings[2].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
		layoutBindings[2].descriptorCount = MAX_TEXTURES;
		layoutBindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		layoutBindings[2].pImmutableSamplers = nullptr;
		BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Added descriptor of type VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE at binding 2");

		// Sampler
		layoutBindings[3].binding = 3;
		layoutBindings[3].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
		layoutBindings[3].descriptorCount = 1;
		layoutBindings[3].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		layoutBindings[3].pImmutableSamplers = nullptr;
		BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Added descriptor of type VK_DESCRIPTOR_TYPE_SAMPLER at binding 3");

		VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
		layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
		layoutCreateInfo.bindingCount = static_cast<uint32>(layoutBindings.size());
		layoutCreateInfo.pBindings = layoutBindings.data();

		if (vkCreateDescriptorSetLayout(_logicalDevice, &layoutCreateInfo, nullptr, &m_DescriptorSetLayout) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create descriptor set layout");
		}

		BE_LOG(LogCategory::Info, "[DESCRIPTOR SET LAYOUT]: Created descriptor set layout");
	}

	VulkanDescriptorSetLayout::~VulkanDescriptorSetLayout()
	{
		if (m_DescriptorSetLayout == VK_NULL_HANDLE)
			return;

		vkDestroyDescriptorSetLayout(m_LogicalDevice, m_DescriptorSetLayout, nullptr);
		m_DescriptorSetLayout = VK_NULL_HANDLE;
	}
}