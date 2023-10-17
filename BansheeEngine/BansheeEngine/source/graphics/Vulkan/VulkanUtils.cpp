#include "VulkanUtils.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	void VulkanUtils::CheckInstanceExtSupport(const std::vector<const char*>& _requiredExtensions)
	{
		unsigned int extensionCount = 0;
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

		bool found = false;

		for (int i = 0; i < _requiredExtensions.size(); ++i)
		{
			for (const auto& extension : extensions)
			{
				found = false;

				if (std::strcmp(_requiredExtensions[i], extension.extensionName) == 0)
				{
					found = true;
					break;
				}
			}
		}
	}

	void VulkanUtils::CheckInstanceLayerSupport(const std::vector<const char*>& _requiredLayers)
	{
		unsigned int layerCount = 0;
		vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
		std::vector<VkLayerProperties> layers(layerCount);
		vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

		bool found = false;

		for (int i = 0; i < _requiredLayers.size(); ++i)
		{
			for (const auto& layer : layers)
			{
				found = false;

				if (std::strcmp(_requiredLayers[i], layer.layerName) == 0)
				{
					found = true;
					break;
				}
			}
		}
	}

	void VulkanUtils::CheckDeviceExtSupport(const VkPhysicalDevice& _gpu, const std::vector<const char*>& _requiredExtensions)
	{
		unsigned int extensionCount = 0;
		vkEnumerateDeviceExtensionProperties(_gpu, nullptr, &extensionCount, nullptr);
		std::vector<VkExtensionProperties> extensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(_gpu, nullptr, &extensionCount, extensions.data());

		bool found = false;

		for (int i = 0; i < _requiredExtensions.size(); ++i)
		{
			for (const auto& extension : extensions)
			{
				found = false;

				if (std::strcmp(_requiredExtensions[i], extension.extensionName) == 0)
				{
					found = true;
					break;
				}
			}
		}
	}

	VkShaderModule VulkanUtils::CreateShaderModule(const VkDevice& _logicalDevice, const std::vector<char>& _shaderBinaryCode)
	{
		VkShaderModuleCreateInfo shaderCreateInfo{};
		shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
		shaderCreateInfo.codeSize = _shaderBinaryCode.size();
		shaderCreateInfo.pCode = reinterpret_cast<const unsigned int*>(_shaderBinaryCode.data());

		VkShaderModule shaderModule = VK_NULL_HANDLE;
		if (vkCreateShaderModule(_logicalDevice, &shaderCreateInfo, nullptr, &shaderModule) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a shader module");
		}

		return shaderModule;
	}

	VkImageView VulkanUtils::CreateImageView(const VkDevice& _logicalDevice, const VkImage& _image, const uint32_t _format, const uint32_t _aspect)
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
		imageViewCreateInfo.subresourceRange.aspectMask = _aspect;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		VkImageView imageView = VK_NULL_HANDLE;
		if (vkCreateImageView(_logicalDevice, &imageViewCreateInfo, nullptr, &imageView) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a Vulkan image view object");
		}

		return imageView;
	}

	void VulkanUtils::CreateBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint64_t _size, const uint32_t _usage, const uint32_t _memoryPropertyFlags, VkBuffer& _buffer, VkDeviceMemory& _bufferMemory)
	{
		// Create buffer object
		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = _size;
		bufferCreateInfo.usage = _usage;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(_logicalDevice, &bufferCreateInfo, nullptr, &_buffer) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create a vertex buffer");
		}

		// Allocate memory for the buffer
		VkMemoryRequirements memRequirements{};
		vkGetBufferMemoryRequirements(_logicalDevice, _buffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanUtils::FindMemoryTypeIndex(_gpu, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(_logicalDevice, &allocInfo, nullptr, &_bufferMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to allocated memory for a vertex buffer");
		}

		vkBindBufferMemory(_logicalDevice, _buffer, _bufferMemory, 0);
	}

	void VulkanUtils::CreateImage(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint32_t _w, const uint32_t _h, const VkFormat _format, const VkImageTiling _tiling, const VkImageUsageFlagBits _usage, const uint32_t _memoryPropertyFlags, VkImage& _image, VkDeviceMemory& _imageMemory)
	{
		// Create image object
		VkImageCreateInfo imageCreateInfo{};
		imageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
		imageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
		imageCreateInfo.extent.width = _w;
		imageCreateInfo.extent.height = _h;
		imageCreateInfo.extent.depth = 1;
		imageCreateInfo.mipLevels = 1;
		imageCreateInfo.arrayLayers = 1;
		imageCreateInfo.format = _format;
		imageCreateInfo.tiling = _tiling;
		imageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		imageCreateInfo.usage = _usage;
		imageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
		imageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateImage(_logicalDevice, &imageCreateInfo, nullptr, &_image) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to create image");
		}

		// Allocate memory for image
		VkMemoryRequirements memReq{};
		vkGetImageMemoryRequirements(_logicalDevice, _image, &memReq);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memReq.size;
		allocInfo.memoryTypeIndex = FindMemoryTypeIndex(_gpu, memReq.memoryTypeBits, _memoryPropertyFlags);

		if (vkAllocateMemory(_logicalDevice, &allocInfo, nullptr, &_imageMemory) != VK_SUCCESS)
		{
			throw std::runtime_error("ERROR: Failed to allocate memory for image");
		}

		// Connect memory to image
		vkBindImageMemory(_logicalDevice, _image, _imageMemory, 0);
	}

	uint32_t VulkanUtils::FindMemoryTypeIndex(const VkPhysicalDevice& _gpu, const uint32_t _memoryTypeBits, const uint32_t _memoryPropertyFlags)
	{
		VkPhysicalDeviceMemoryProperties memoryProperties{};
		vkGetPhysicalDeviceMemoryProperties(_gpu, &memoryProperties);

		for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; ++i)
		{
			if ((_memoryTypeBits & (1 << i)) &&
				(memoryProperties.memoryTypes[i].propertyFlags & _memoryPropertyFlags) == _memoryPropertyFlags)
			{
				return i;
			}
		}

		return UINT32_MAX;
	}

	void VulkanUtils::CopyBuffer(const VkDevice& _logicalDevice, const VkCommandPool& _cmdPool, const VkQueue& _queue, const uint64_t _size, const VkBuffer& _srcBuffer, const VkBuffer& _dstBuffer)
	{
		// Allocate command buffer
		VkCommandBufferAllocateInfo cmdBufferAllocInfo{};
		cmdBufferAllocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		cmdBufferAllocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		cmdBufferAllocInfo.commandPool = _cmdPool;
		cmdBufferAllocInfo.commandBufferCount = 1;

		VkCommandBuffer cmdBuffer{};
		vkAllocateCommandBuffers(_logicalDevice, &cmdBufferAllocInfo, &cmdBuffer);

		// Record into command buffer
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

		vkBeginCommandBuffer(cmdBuffer, &beginInfo);

		VkBufferCopy bufferCopy{};
		bufferCopy.srcOffset = 0;
		bufferCopy.dstOffset = 0;
		bufferCopy.size = _size;
		vkCmdCopyBuffer(cmdBuffer, _srcBuffer, _dstBuffer, 1, &bufferCopy);

		vkEndCommandBuffer(cmdBuffer);

		// Submit command buffer onto queue
		VkSubmitInfo submitInfo{};
		submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
		submitInfo.commandBufferCount = 1;
		submitInfo.pCommandBuffers = &cmdBuffer;

		vkQueueSubmit(_queue, 1, &submitInfo, VK_NULL_HANDLE);
		vkQueueWaitIdle(_queue);

		vkFreeCommandBuffers(_logicalDevice, _cmdPool, 1, &cmdBuffer);
	}

	VkFormat VulkanUtils::FindSupportedFormat(const VkPhysicalDevice& _gpu, const std::vector<VkFormat>& _formats, const VkImageTiling _tiling, const uint32_t _formatFeatures)
	{
		for (VkFormat format : _formats)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(_gpu, format, &props);

			if (_tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & _formatFeatures) == _formatFeatures)
			{
				return format;
			}
			else if (_tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & _formatFeatures) == _formatFeatures)
			{
				return format;
			}
		}

		throw std::runtime_error("ERROR: Failed to find supported format");
	}

	bool VulkanUtils::HasStencilComponent(const VkFormat _format)
	{
		return _format == VK_FORMAT_D32_SFLOAT_S8_UINT || _format == VK_FORMAT_D24_UNORM_S8_UINT;
	}
}
