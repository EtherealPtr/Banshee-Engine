#include "VulkanVertexBufferManager.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Graphics/Components/MeshComponent.h"
#include "Graphics/ShapeFactory.h"
#include "Graphics/Systems/ModelLoadingSystem.h"
#include "Graphics/Systems/MeshSystem.h"
#include <stdexcept>

namespace Banshee
{
	VulkanVertexBufferManager::VulkanVertexBufferManager(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue) :
		m_LogicalDevice(_logicalDevice),
		m_PhysicalDevice(_physicalDevice),
		m_CommandPool(_commandPool),
		m_GraphicsQueue(_graphicsQueue),
		m_VertexBuffers{}
	{}

	void VulkanVertexBufferManager::GenerateBuffers(const uint32 _bufferId, void* _vertexData, const uint64 _sizeOfVertexData, void* _indexData, const uint64 _sizeOfIndexData)
	{
		m_VertexBuffers.emplace
		(
			std::piecewise_construct,
			std::forward_as_tuple(_bufferId),
			std::forward_as_tuple(m_LogicalDevice, m_PhysicalDevice, m_CommandPool, m_GraphicsQueue, _vertexData, _sizeOfVertexData, _indexData, _sizeOfIndexData)
		);
	}

	void VulkanVertexBufferManager::CreateBasicShapeVertexBuffer(MeshComponent* _meshComponent)
	{
		assert(_meshComponent != nullptr);

		const uint32 meshId = _meshComponent->GetMeshId();
		auto vertexBuffer = m_VertexBuffers.find(meshId);
		
		if (vertexBuffer != m_VertexBuffers.end())
		{
			const auto duplicatedMesh = MeshSystem::Instance().GetMeshComponentById(meshId);
			_meshComponent->SetSubMeshes(duplicatedMesh->GetSubMeshes());
			return;
		}
		else
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32> indices{};
		
			ShapeFactory::GetShapeData(static_cast<PrimitiveShape>(meshId), vertices, indices);
			Mesh mesh{};
			mesh.vertices = vertices;
			mesh.indices = indices;
			mesh.SetTexId(_meshComponent->GetTexId());
			mesh.material.SetDiffuseColor(_meshComponent->GetColor());
			_meshComponent->SetSubMesh(mesh);

			GenerateBuffers(meshId, vertices.data(), sizeof(Vertex) * vertices.size(), indices.data(), sizeof(uint32) * indices.size());
		}
	}

	void VulkanVertexBufferManager::CreateModelVertexBuffer(MeshComponent* _meshComponent)
	{
		assert(_meshComponent != nullptr);

		const std::string modelName = _meshComponent->GetModelName();
		uint32 modelId = 0;

		auto it = m_ModelNameToIdMap.find(modelName);
		if (it != m_ModelNameToIdMap.end())
		{
			modelId = it->second;
			_meshComponent->SetMeshId(modelId);

			const auto duplicatedMesh = MeshSystem::Instance().GetMeshComponentById(modelId);
			_meshComponent->SetSubMeshes(duplicatedMesh->GetSubMeshes());
			return;
		}
		else
		{
			modelId = static_cast<uint32>(m_ModelNameToIdMap.size());
			constexpr uint32 modelIdOffset = 1000;
			modelId += modelIdOffset;
			m_ModelNameToIdMap[modelName] = modelId;
			_meshComponent->SetMeshId(modelId);
		}

		auto vertexBuffer = m_VertexBuffers.find(modelId);
		if (vertexBuffer != m_VertexBuffers.end())
		{
			return;
		}

		std::vector<Vertex> vertices{};
		std::vector<uint32> indices{};
		const ModelLoadingSystem modelLoadingSystem(_meshComponent->GetModelPath().c_str(), _meshComponent, vertices, indices);

		GenerateBuffers(modelId, vertices.data(), sizeof(Vertex) * vertices.size(), indices.data(), sizeof(uint32) * indices.size());
	}

	VulkanVertexBuffer* VulkanVertexBufferManager::GetVertexBuffer(const uint32 _meshId)
	{
		auto vertexBuffer = m_VertexBuffers.find(_meshId);
		if (vertexBuffer != m_VertexBuffers.end())
		{
			return &vertexBuffer->second;
		}
		else
		{
			BE_LOG(LogCategory::Error, "[VERTEX MANAGER]: Failed to find vertex buffer with id: %d", _meshId);
			throw std::runtime_error("Mesh id not found in vertex buffers map");
		}
	}
} // End of Banshee namespace