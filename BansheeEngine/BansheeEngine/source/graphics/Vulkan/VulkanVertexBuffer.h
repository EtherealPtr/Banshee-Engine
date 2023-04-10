#pragma once

#include "foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkCommandBuffer_T* VkCommandBuffer;
typedef struct VkBuffer_T* VkBuffer;
typedef struct VkDeviceMemory_T* VkDeviceMemory;

namespace Banshee
{
	class VulkanVertexBuffer
	{
	public:
		VulkanVertexBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, void* _data, const uint64 _size);
		~VulkanVertexBuffer();

		void Bind(const VkCommandBuffer& _commandBuffer);
		VkBuffer Get() const { return m_VertexBuffer; }

	private:
		VkDevice m_LogicalDevice;
		VkBuffer m_VertexBuffer;
		VkDeviceMemory m_VertexBufferMemory;
	};
} // End of Banshee namespace
