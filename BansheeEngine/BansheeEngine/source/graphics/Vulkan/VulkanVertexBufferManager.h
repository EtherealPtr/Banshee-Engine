#pragma once

#include "VulkanVertexBuffer.h"
#include <unordered_map>

namespace Banshee
{
	class VulkanVertexBufferManager
	{
	public:
		VulkanVertexBufferManager(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue);

		void GenerateBuffers(const uint32_t _bufferId, void* _vertexData, const uint64_t _sizeOfVertexData, void* _indexData, const uint64_t _sizeOfIndexData);
		void Bind(const uint32_t bufferId, const VkCommandBuffer& _commandBuffer);
		uint32_t GetCurrentIndicesCount() const { return m_CurrentIndicesCount; }

	private:
		VkDevice m_LogicalDevice;
		VkPhysicalDevice m_PhysicalDevice;
		VkCommandPool m_CommandPool;
		VkQueue m_GraphicsQueue;
		uint32_t m_CurrentIndicesCount;
		std::unordered_map<uint32_t /*bufferId*/, VulkanVertexBuffer /*bufferData*/> m_VertexBuffers;
	};
}
