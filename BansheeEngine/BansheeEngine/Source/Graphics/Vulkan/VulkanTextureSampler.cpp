#include "VulkanTextureSampler.h"
#include "Foundation/Logging/Logger.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanTextureSampler::VulkanTextureSampler(const VkDevice& _device, const VkPhysicalDevice& _gpu) : 
		m_LogicalDevice(_device),
		m_PhysicalDevice(_gpu),
		m_TextureSampler(VK_NULL_HANDLE)
	{
		BE_LOG(LogCategory::Trace, "[SAMPLER]: Creating texture sampler");

		VkSamplerCreateInfo samplerCreateInfo{};
		samplerCreateInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
		samplerCreateInfo.magFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.minFilter = VK_FILTER_LINEAR;
		samplerCreateInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT;
		samplerCreateInfo.unnormalizedCoordinates = VK_FALSE;
		samplerCreateInfo.compareEnable = VK_FALSE;
		samplerCreateInfo.compareOp = VK_COMPARE_OP_ALWAYS;
		samplerCreateInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
		samplerCreateInfo.mipLodBias = 0.0f;
		samplerCreateInfo.minLod = 0.0f;
		samplerCreateInfo.maxLod = 0.0f;

		VkPhysicalDeviceProperties deviceProperties;
		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceProperties(m_PhysicalDevice, &deviceProperties);
		vkGetPhysicalDeviceFeatures(m_PhysicalDevice, &deviceFeatures);

		if (deviceFeatures.samplerAnisotropy)
		{
			samplerCreateInfo.anisotropyEnable = VK_TRUE;
			samplerCreateInfo.maxAnisotropy = deviceProperties.limits.maxSamplerAnisotropy;
		}
		else
		{
			samplerCreateInfo.anisotropyEnable = VK_FALSE;
			samplerCreateInfo.maxAnisotropy = 1.0f;
		}

		if (vkCreateSampler(m_LogicalDevice, &samplerCreateInfo, nullptr, &m_TextureSampler) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create texture sampler\n");
		}

		BE_LOG(LogCategory::Info, "[SAMPLER]: Created texture sampler");
	}

	VulkanTextureSampler::~VulkanTextureSampler()
	{
		vkDestroySampler(m_LogicalDevice, m_TextureSampler, nullptr);
	}
}