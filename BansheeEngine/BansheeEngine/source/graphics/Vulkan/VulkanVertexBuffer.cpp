#include "VulkanVertexBuffer.h"
#include "foundation/Logger.h"
#include "VulkanUtils.h"
#include <vulkan/vulkan.h>
#include <stdexcept>

namespace Banshee
{
	VulkanVertexBuffer::VulkanVertexBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, void* _data, const uint64 _size) :
		m_LogicalDevice(_logicalDevice)
	{
		BE_LOG(LogCategory::trace, "Vulkan vertex buffer creation stage: Start");

		// Create buffer object
		VkBufferCreateInfo bufferCreateInfo{};
		bufferCreateInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
		bufferCreateInfo.size = _size;
		bufferCreateInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
		bufferCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

		if (vkCreateBuffer(_logicalDevice, &bufferCreateInfo, nullptr, &m_VertexBuffer) != VK_SUCCESS)
		{
			BE_LOG(LogCategory::error, "Failed to create a vertex buffer");
			throw std::runtime_error("ERROR: Failed to create a vertex buffer");
		}

		// Allocate memory for the buffer
		VkMemoryRequirements memRequirements{};
		vkGetBufferMemoryRequirements(_logicalDevice, m_VertexBuffer, &memRequirements);

		VkMemoryAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
		allocInfo.allocationSize = memRequirements.size;
		allocInfo.memoryTypeIndex = VulkanUtils::FindMemoryTypeIndex(_physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

		if (vkAllocateMemory(_logicalDevice, &allocInfo, nullptr, &m_VertexBufferMemory) != VK_SUCCESS)
		{
			BE_LOG(LogCategory::error, "Failed to allocated memory for a vertex buffer");
			throw std::runtime_error("ERROR: Failed to allocated memory for a vertex buffer");
		}

		vkBindBufferMemory(_logicalDevice, m_VertexBuffer, m_VertexBufferMemory, 0);

		// Map the memory
		void* mappedData = nullptr;
		vkMapMemory(_logicalDevice, m_VertexBufferMemory, 0, _size, 0, &mappedData);
		
		// Copy the vertex data to the buffer
		memcpy(mappedData, _data, _size);

		// Unmap the memory
		vkUnmapMemory(_logicalDevice, m_VertexBufferMemory);

		BE_LOG(LogCategory::info, "Vulkan vertex buffer created successfully");
		BE_LOG(LogCategory::trace, "Vulkan vertex buffer creation stage: End");
	}

	VulkanVertexBuffer::~VulkanVertexBuffer()
	{
		vkFreeMemory(m_LogicalDevice, m_VertexBufferMemory, nullptr);
		vkDestroyBuffer(m_LogicalDevice, m_VertexBuffer, nullptr);
		m_VertexBufferMemory = VK_NULL_HANDLE;
		m_VertexBuffer = VK_NULL_HANDLE;
		BE_LOG(LogCategory::trace, "Vulkan vertex buffer destroyed");
	}

	void VulkanVertexBuffer::Bind(const VkCommandBuffer& _commandBuffer)
	{
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(_commandBuffer, 0, 1, &m_VertexBuffer, offsets);
	}
} // End of Banshee namespace