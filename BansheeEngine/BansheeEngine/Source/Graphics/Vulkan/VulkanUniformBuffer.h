#pragma once

#include "Foundation/Platform.h"

typedef struct VkDevice_T* VkDevice;
typedef struct VkPhysicalDevice_T* VkPhysicalDevice;
typedef struct VkBuffer_T* VkBuffer;
typedef struct VkDeviceMemory_T* VkDeviceMemory;

namespace Banshee
{
	class VulkanUniformBuffer
	{
	public:
		VulkanUniformBuffer(const VkDevice& _logicalDevice, const VkPhysicalDevice& _gpu, const uint64 _size);
		~VulkanUniformBuffer();

		void CopyData(void* _pData) const noexcept;
		VkBuffer GetBuffer() const noexcept { return m_Buffer; }
		uint64 GetBufferSize() const noexcept { return m_BufferSize; }
		VkDeviceMemory GetBufferMemory() const noexcept { return m_BufferMemory; }

	private:
		VkDevice m_LogicalDevice;
		VkBuffer m_Buffer;
		VkDeviceMemory m_BufferMemory;
		uint64 m_BufferSize;
	};
} // End of Banshee namespace