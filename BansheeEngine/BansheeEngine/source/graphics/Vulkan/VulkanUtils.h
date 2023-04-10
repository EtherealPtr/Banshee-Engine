#pragma once

#include "foundation/Platform.h"
#include <vector>

typedef struct VkDevice_T* VkDevice;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkImage_T* VkImage;
typedef struct VkImageView_T* VkImageView;
typedef struct VkShaderModule_T* VkShaderModule;

namespace Banshee
{
	class VulkanUtils
	{
	public:
		static void CheckInstanceExtSupport(const std::vector<const char*>& _requiredExtensions);
		static void CheckInstanceLayerSupport(const std::vector<const char*>& _requiredLayers);
		static void CheckDeviceExtSupport(const VkPhysicalDevice& _gpu, const std::vector<const char*>& _requiredExtensions);
		static VkShaderModule CreateShaderModule(const VkDevice& _logicalDevice, const std::vector<char>& _shaderBinaryCode);
		static VkImageView CreateImageView(const VkDevice& _logicalDevice, const VkImage& _image, const uint32 _format);
		static uint32 FindMemoryTypeIndex(const VkPhysicalDevice& _gpu, const uint32 _memoryTypeBits, const uint32 _memoryPropertyFlags);
	};

} // End of Banshee namespace
