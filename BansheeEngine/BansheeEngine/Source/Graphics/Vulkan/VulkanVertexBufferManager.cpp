#include "VulkanVertexBufferManager.h"
#include "Foundation/Entity/Entity.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Graphics/Components/Mesh/PrimitiveMeshComponent.h"
#include "Graphics/Components/Mesh/CustomMeshComponent.h"
#include "Graphics/Shapes/ShapeFactory.h"
#include "Graphics/Systems/ModelLoadingSystem.h"
#include "Graphics/Systems/MeshSystem.h"
#include <algorithm>

namespace Banshee
{
	uint32 VulkanVertexBufferManager::s_NextBufferId{ 0 };

	VulkanVertexBufferManager::VulkanVertexBufferManager(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue) :
		m_LogicalDevice{ _logicalDevice },
		m_PhysicalDevice{ _physicalDevice },
		m_CommandPool{ _commandPool },
		m_GraphicsQueue{ _graphicsQueue },
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

	void VulkanVertexBufferManager::CreateBasicShapeVertexBuffer(PrimitiveMeshComponent& _meshComponent, MeshSystem& _meshSystem)
	{
		const uint32 bufferId{ ++s_NextBufferId };
		_meshComponent.SetVertexBufferId(bufferId);

		std::vector<Vertex> vertices{};
		std::vector<uint32> indices{};
		ShapeFactory::GetShapeData(_meshComponent.GetShape(), vertices, indices);
		_meshComponent.SetIndices(static_cast<uint32>(indices.size()));

		GenerateBuffers(bufferId, vertices.data(), sizeof(Vertex) * vertices.size(), indices.data(), sizeof(uint32) * indices.size());
		_meshComponent.GetMeshData().SetVertexBufferId(_meshComponent.GetVertexBufferId());
		_meshSystem.AddMesh(_meshComponent.GetMeshData());
	}

	void VulkanVertexBufferManager::CreateModelVertexBuffer(CustomMeshComponent& _meshComponent, MeshSystem& _meshSystem)
	{
		const std::string_view modelName{ _meshComponent.GetModelName() };

		auto modelBufferId{ m_ModelNameToBufferId.find(modelName.data()) };
		if (modelBufferId != m_ModelNameToBufferId.end())
		{
			_meshComponent.SetVertexBufferId(modelBufferId->second);

			const auto& originalSubMeshes{ _meshSystem.GetSubMeshes(modelBufferId->second) };
			std::vector<MeshData> duplicatedMeshes(originalSubMeshes.size());

			std::transform(originalSubMeshes.begin(), originalSubMeshes.end(), duplicatedMeshes.begin(), [&](const MeshData& _subMesh)
				{
					MeshData copiedSubMesh{ _subMesh };
					copiedSubMesh.SetEntityId(_meshComponent.GetOwner()->GetUniqueId());
					copiedSubMesh.SetShaderType(_meshComponent.GetShaderType());

					if (_meshComponent.GetTintColor().has_value())
					{
						const glm::vec3& tintColor{ _meshComponent.GetTintColor().value() };
						copiedSubMesh.SetDiffuseColor(tintColor);
						copiedSubMesh.SetSpecularColor(tintColor);
					}

					return copiedSubMesh;
				});

			_meshSystem.AddMeshes(duplicatedMeshes);
			return;
		}

		const uint32 bufferId{ ++s_NextBufferId };
		_meshComponent.SetVertexBufferId(bufferId);
		m_ModelNameToBufferId[modelName.data()] = bufferId;

		std::vector<Vertex> vertices{};
		std::vector<uint32> indices{};
		const ModelLoadingSystem modelLoadingSystem{ _meshComponent, _meshSystem, vertices, indices };

		GenerateBuffers(bufferId, vertices.data(), sizeof(Vertex) * vertices.size(), indices.data(), sizeof(uint32) * indices.size());
	}

	VulkanVertexBuffer* VulkanVertexBufferManager::GetVertexBuffer(const uint32 _bufferId)
	{
		auto vertexBuffer = m_VertexBuffers.find(_bufferId);
		if (vertexBuffer != m_VertexBuffers.end())
		{
			return &vertexBuffer->second;
		}
		else
		{
			BE_LOG(LogCategory::Error, "[VERTEX MANAGER]: Failed to find vertex buffer with id: %d", _bufferId);
			throw std::runtime_error("Mesh id not found in vertex buffers map");
		}
	}
} // End of Banshee namespace