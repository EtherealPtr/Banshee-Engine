#include "VulkanVertexBufferManager.h"
#include "../ShapeFactory.h"

namespace Banshee
{
	VulkanVertexBufferManager::VulkanVertexBufferManager(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue) :
		m_LogicalDevice(_logicalDevice),
		m_PhysicalDevice(_physicalDevice),
		m_CommandPool(_commandPool),
		m_GraphicsQueue(_graphicsQueue),
		m_CurrentIndicesCount(0),
		m_VertexBuffers{}
	{}

	void VulkanVertexBufferManager::GenerateBuffers(const uint32 _bufferId, void* _vertexData, const uint64 _sizeOfVertexData, void* _indexData, const uint64 _sizeOfIndexData)
	{
		m_VertexBuffers.emplace(std::piecewise_construct,
			std::forward_as_tuple(_bufferId),
			std::forward_as_tuple(m_LogicalDevice, m_PhysicalDevice, m_CommandPool, m_GraphicsQueue, _vertexData, _sizeOfVertexData, _indexData, _sizeOfIndexData));
	}

	void VulkanVertexBufferManager::Bind(const uint32 _bufferId, const VkCommandBuffer& _commandBuffer)
	{
		auto vertexBuffer = m_VertexBuffers.find(_bufferId);
		if (vertexBuffer != m_VertexBuffers.end())
		{
			m_CurrentIndicesCount = ShapeFactory::GetIndicesCount(static_cast<PrimitiveShape>(_bufferId));
			vertexBuffer->second.Bind(_commandBuffer);
		}
		else
		{
			std::vector<Vertex> vertices{};
			std::vector<uint16> indices{};

			ShapeFactory::GetVertices(static_cast<PrimitiveShape>(_bufferId), vertices, indices);
			GenerateBuffers(_bufferId, vertices.data(), sizeof(Vertex) * vertices.size(), indices.data(), sizeof(uint16) * indices.size());
			Bind(_bufferId, _commandBuffer);
		}
	}
}