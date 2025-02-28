#include "VulkanDescriptorSetLayout.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/ResourceManager/Image/ImageManager.h"
#include <vulkan/vulkan.h>

namespace Banshee
{
	VulkanDescriptorSetLayout::VulkanDescriptorSetLayout(const VkDevice& _logicalDevice, const SceneDataEnum _descriptorSetLayoutType) :
		m_LogicalDevice{ _logicalDevice },
		m_DescriptorSetLayout{ VK_NULL_HANDLE }
	{
		BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Creating descriptor set layout");

		std::vector<VkDescriptorSetLayoutBinding> layoutBindings;

		if (_descriptorSetLayoutType == SceneDataEnum::Scene)
		{
			layoutBindings.resize(7);

			// View-projection binding
			layoutBindings[0].binding = 0;
			layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			layoutBindings[0].descriptorCount = 1;
			layoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			layoutBindings[0].pImmutableSamplers = nullptr;
			BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Added VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER at binding 0");

			// Color dynamic buffer
			layoutBindings[1].binding = 1;
			layoutBindings[1].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
			layoutBindings[1].descriptorCount = 1;
			layoutBindings[1].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			layoutBindings[1].pImmutableSamplers = nullptr;
			BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Added VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC at binding 1");

			// Textures
			constexpr uint32 maxTextures{ 16 };
			layoutBindings[2].binding = 2;
			layoutBindings[2].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE;
			layoutBindings[2].descriptorCount = maxTextures;
			layoutBindings[2].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			layoutBindings[2].pImmutableSamplers = nullptr;
			BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Added VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE at binding 2");

			// Sampler
			layoutBindings[3].binding = 3;
			layoutBindings[3].descriptorType = VK_DESCRIPTOR_TYPE_SAMPLER;
			layoutBindings[3].descriptorCount = 1;
			layoutBindings[3].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			layoutBindings[3].pImmutableSamplers = nullptr;
			BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Added VK_DESCRIPTOR_TYPE_SAMPLER at binding 3");

			// Light storage buffer
			layoutBindings[4].binding = 4;
			layoutBindings[4].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
			layoutBindings[4].descriptorCount = 1;
			layoutBindings[4].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			layoutBindings[4].pImmutableSamplers = nullptr;
			BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Added VK_DESCRIPTOR_TYPE_STORAGE_BUFFER at binding 4");

			// Light space matrix for sampling
			layoutBindings[5].binding = 5;
			layoutBindings[5].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			layoutBindings[5].descriptorCount = 1;
			layoutBindings[5].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			layoutBindings[5].pImmutableSamplers = nullptr;
			BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Added VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER at binding 5");

			// Binding 6: Shadow map (combined image sampler)
			layoutBindings[6].binding = 6;
			layoutBindings[6].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			layoutBindings[6].descriptorCount = 1;
			layoutBindings[6].stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
			layoutBindings[6].pImmutableSamplers = nullptr;
			BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Added VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER at binding 6");
		}
		else if (_descriptorSetLayoutType == SceneDataEnum::DepthOnly)
		{
			layoutBindings.resize(1);

			// Shadow map uniform buffer (lightSpaceMatrix)
			layoutBindings[0].binding = 0;
			layoutBindings[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			layoutBindings[0].descriptorCount = 1;
			layoutBindings[0].stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
			layoutBindings[0].pImmutableSamplers = nullptr;
			BE_LOG(LogCategory::Trace, "[DESCRIPTOR SET LAYOUT]: Added VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER at binding 0 for DepthOnly");
		}

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
		{
			return;
		}

		vkDestroyDescriptorSetLayout(m_LogicalDevice, m_DescriptorSetLayout, nullptr);
		m_DescriptorSetLayout = VK_NULL_HANDLE;
	}
} // End of namespace