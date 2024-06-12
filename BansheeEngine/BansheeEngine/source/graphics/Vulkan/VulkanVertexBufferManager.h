#pragma once

#include "VulkanVertexBuffer.h"
#include "Foundation/Platform.h"
#include <unordered_map>
#include <string>

namespace Banshee
{
	class MeshComponent;

	class VulkanVertexBufferManager
	{
	public:
		VulkanVertexBufferManager(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue);

		void GenerateBuffers(const uint32 _bufferId, void* _vertexData, const uint64 _sizeOfVertexData, void* _indexData, const uint64 _sizeOfIndexData);
		void CreateBasicShapeVertexBuffer(MeshComponent* _meshComponent);
		void CreateModelVertexBuffer(MeshComponent* _meshComponent);
		VulkanVertexBuffer* GetVertexBuffer(const uint32 _bufferId);

	private:
		VkDevice m_LogicalDevice;
		VkPhysicalDevice m_PhysicalDevice;
		VkCommandPool m_CommandPool;
		VkQueue m_GraphicsQueue;
		std::unordered_map<uint32, VulkanVertexBuffer> m_VertexBuffers;
		std::unordered_map<std::string, uint32> m_ModelNameToIdMap;
	};
} // End of Banshee namespace