#pragma once

#include <vector>
#include <cstdint>

typedef struct VkDevice_T* VkDevice;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkImage_T* VkImage;
typedef struct VkImageView_T* VkImageView;
typedef struct VkShaderModule_T* VkShaderModule;
typedef struct VkBuffer_T* VkBuffer;
typedef struct VkDeviceMemory_T* VkDeviceMemory;
typedef struct VkCommandPool_T* VkCommandPool;
typedef struct VkQueue_T* VkQueue;
typedef enum VkFormat VkFormat;
typedef enum VkImageTiling VkImageTiling;
typedef enum VkImageUsageFlagBits VkImageUsageFlagBits;

namespace Banshee
{
	class VulkanUtils
	{
	public:
		static void CheckInstanceExtSupport(const std::vector<const char*>& _requiredExtensions);
		static void CheckInstanceLayerSupport(const std::vector<const char*>& _requiredLayers);
		static void CheckDeviceExtSupport(const VkPhysicalDevice& _gpu, const std::vector<const char*>& _requiredExtensions);
		static VkShaderModule CreateShaderModule(const VkDevice& _logicalDevice, const std::vector<char>& _shaderBinaryCode);
		static void CreateBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint64_t _size, const uint32_t _usage, const uint32_t _memoryPropertyFlags, VkBuffer& _buffer, VkDeviceMemory& _bufferMemory);
		static void CreateImage(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint32_t _w, const uint32_t _h, const VkFormat _format, const VkImageTiling _tiling, const VkImageUsageFlagBits _usage, const uint32_t _memoryPropertyFlags, VkImage& _image, VkDeviceMemory& _imageMemory);
		static VkImageView CreateImageView(const VkDevice& _logicalDevice, const VkImage& _image, const uint32_t _format, const uint32_t _aspect);
		static uint32_t FindMemoryTypeIndex(const VkPhysicalDevice& _gpu, const uint32_t _memoryTypeBits, const uint32_t _memoryPropertyFlags);
		static void CopyBuffer(const VkDevice& _logicalDevice, const VkCommandPool& _cmdPool, const VkQueue& _queue, const uint64_t _size, const VkBuffer& _srcBuffer, const VkBuffer& _dstBuffer);
		static VkFormat FindSupportedFormat(const VkPhysicalDevice& _gpu, const std::vector<VkFormat>& _formats, const VkImageTiling _tiling, const uint32_t _formatFeatures);
		static bool HasStencilComponent(const VkFormat _format);
	};
}
