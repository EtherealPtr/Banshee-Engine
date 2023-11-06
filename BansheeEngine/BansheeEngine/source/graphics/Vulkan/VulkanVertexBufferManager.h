#pragma once

#include "VulkanVertexBuffer.h"
#include "Foundation/Platform.h"
#include <unordered_map>

namespace Banshee
{
	class VulkanVertexBufferManager
	{
	public:
		VulkanVertexBufferManager(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue);

		void GenerateBuffers(const uint32 _bufferId, void* _vertexData, const uint64 _sizeOfVertexData, void* _indexData, const uint64 _sizeOfIndexData);
		void Bind(const uint32 bufferId, const VkCommandBuffer& _commandBuffer);
		uint32 GetCurrentIndicesCount() const { return m_CurrentIndicesCount; }

	private:
		VkDevice m_LogicalDevice;
		VkPhysicalDevice m_PhysicalDevice;
		VkCommandPool m_CommandPool;
		VkQueue m_GraphicsQueue;
		uint32 m_CurrentIndicesCount;
		std::unordered_map<uint32 /*bufferId*/, VulkanVertexBuffer /*bufferData*/> m_VertexBuffers;
	};
}
