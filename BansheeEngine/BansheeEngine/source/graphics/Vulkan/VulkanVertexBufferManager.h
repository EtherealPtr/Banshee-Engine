#pragma once

#include "VulkanVertexBuffer.h"
#include "Foundation/Platform.h"
#include <unordered_map>
#include <string>

namespace Banshee
{
	class VulkanVertexBufferManager
	{
	public:
		VulkanVertexBufferManager(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue);

		VulkanVertexBuffer& GenerateBuffers(const uint32 _bufferId, void* _vertexData, const uint64 _sizeOfVertexData, void* _indexData, const uint64 _sizeOfIndexData);
		void BindBasicShape(const uint32 _bufferId, const VkCommandBuffer& _commandBuffer);
		void BindModel(const VkCommandBuffer& _commandBuffer, const std::string& _modelPath);
		uint32 GetCurrentIndicesCount() const noexcept { return m_CurrentIndicesCount; }

	private:
		VkDevice m_LogicalDevice;
		VkPhysicalDevice m_PhysicalDevice;
		VkCommandPool m_CommandPool;
		VkQueue m_GraphicsQueue;
		uint32 m_CurrentIndicesCount;
		std::unordered_map<uint32, VulkanVertexBuffer> m_VertexBuffers;
		std::unordered_map<std::string, uint32> m_ModelNameToIdMap;
	};
} // End of Banshee namespace