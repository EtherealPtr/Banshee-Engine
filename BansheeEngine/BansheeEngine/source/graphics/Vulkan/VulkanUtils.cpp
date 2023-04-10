#include "VulkanUtils.h"
#include "foundation/Platform.h"
#include "foundation/Logger.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	void VulkanUtils::CheckInstanceExtSupport(const std::vector<const char*>& _requiredExtensions)
	{
		uint32 extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());
		
		BE_LOG(LogCategory::trace, "Checking Vulkan instance extensions...");

		bool found = false;

		for (uint8 i = 0; i < _requiredExtensions.size(); ++i)
		{
			for (const auto& extension : extensions)
			{
				found = false;

				if (std::strcmp(_requiredExtensions[i], extension.extensionName) == 0)
				{
					found = true;
					BE_LOG(LogCategory::info, "Vulkan instance extension: %s is supported", _requiredExtensions[i]);
					break;
				}
			}

			if (!found)
			{
				BE_LOG(LogCategory::warning, "Vulkan instance extension: %s is not supported", _requiredExtensions[i]);
			}
		}
	}
	
	void VulkanUtils::CheckInstanceLayerSupport(const std::vector<const char*>& _requiredLayers)
	{
		uint32 layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> layers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		BE_LOG(LogCategory::trace, "Checking Vulkan instance layers...");

		bool found = false;

		for (uint8 i = 0; i < _requiredLayers.size(); ++i)
		{
			for (const auto& layer : layers)
			{
				found = false;

				if (std::strcmp(_requiredLayers[i], layer.layerName) == 0)
				{
					found = true;
					BE_LOG(LogCategory::info, "Vulkan instance layer: %s is supported", _requiredLayers[i]);
					break;
				}
			}

			if (!found)
			{
				BE_LOG(LogCategory::warning, "Vulkan instance extension: %s is not supported", _requiredLayers[i]);
			}
		}
	}
	
	void VulkanUtils::CheckDeviceExtSupport(const VkPhysicalDevice& _gpu, const std::vector<const char*>& _requiredExtensions)
	{
		uint32 extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(_gpu, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(_gpu, nullptr, &extensionCount, extensions.data());

		BE_LOG(LogCategory::trace, "Checking Vulkan device extensions...");

		bool found = false;

		for (uint8 i = 0; i < _requiredExtensions.size(); ++i)
		{
			for (const auto& extension : extensions)
			{
				found = false;

				if (std::strcmp(_requiredExtensions[i], extension.extensionName) == 0)
				{
					found = true;
					BE_LOG(LogCategory::info, "Vulkan device extension: %s is supported", _requiredExtensions[i]);
					break;
				}
			}

			if (!found)
			{
				BE_LOG(LogCategory::warning, "Vulkan device extension: %s is not supported", _requiredExtensions[i]);
			}
		}
	}

	VkShaderModule VulkanUtils::CreateShaderModule(const VkDevice& _logicalDevice, const std::vector<char>& _shaderBinaryCode)
	{
		VkShaderModuleCreateInfo shaderCreateInfo{};
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderCreateInfo.codeSize = _shaderBinaryCode.size();
		shaderCreateInfo.pCode = reinterpret_cast<const uint32*>(_shaderBinaryCode.data());

		VkShaderModule shaderModule = VK_NULL_HANDLE;
		if (vkCreateShaderModule(_logicalDevice, &shaderCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			BE_LOG(LogCategory::warning, "Failed to create a shader module");
		}

		return shaderModule;
	}

	VkImageView VulkanUtils::CreateImageView(const VkDevice& _logicalDevice, const VkImage& _image, const uint32 _format)
	{
		VkImageViewCreateInfo imageViewCreateInfo{};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = _image;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = static_cast<VkFormat>(_format);
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		VkImageView imageView = VK_NULL_HANDLE;
		if (vkCreateImageView(_logicalDevice, &imageViewCreateInfo, nullptr, &imageView) != VK_SUCCESS)
		{
			BE_LOG(LogCategory::warning, "Failed to create a Vulkan image view object");
		}

		return imageView;
	}
	
	uint32 VulkanUtils::FindMemoryTypeIndex(const VkPhysicalDevice& _gpu, const uint32 _memoryTypeBits, const uint32 _memoryPropertyFlags)
	{
		VkPhysicalDeviceMemoryProperties memoryProperties{};
		vkGetPhysicalDeviceMemoryProperties(_gpu, &memoryProperties);

		for (uint32 i = 0; i < memoryProperties.memoryTypeCount; ++i)
		{
			if ((_memoryTypeBits & (1 << i)) &&
			    (memoryProperties.memoryTypes[i].propertyFlags & _memoryPropertyFlags) == _memoryPropertyFlags)
			{
				return i;
			}
		}

		return UINT32_MAX;
	}
} // End of Banshee namespace