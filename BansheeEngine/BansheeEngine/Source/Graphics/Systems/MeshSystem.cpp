#include "MeshSystem.h"
#include "Foundation/Entity/Entity.h"
#include "Graphics/MeshData.h"
#include "Graphics/Components/Mesh/PrimitiveMeshComponent.h"
#include "Graphics/Components/Mesh/CustomMeshComponent.h"
#include "Graphics/Vulkan/VulkanVertexBufferManager.h"
#include <algorithm>

namespace Banshee
{
	MeshSystem::MeshSystem(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue) :
		m_VertexBufferManager{ _logicalDevice, _physicalDevice, _commandPool, _graphicsQueue },
		m_CachedSubMeshes{},
		m_VertexBufferIdToSubMeshes{},
		m_IsCacheDirty{ true },
		m_TotalMeshCount{ 0 }
	{}

	void MeshSystem::ProcessComponents(const std::shared_ptr<Entity>& _entity)
	{
		if (const auto& primitiveMesh{ _entity->GetComponent<PrimitiveMeshComponent>() })
		{
			m_VertexBufferManager.CreateBasicShapeVertexBuffer(*primitiveMesh.get());
			AddMesh(primitiveMesh.get()->GetMeshData());
		}
		else if (const auto& modelMesh{ _entity->GetComponent<CustomMeshComponent>() })
		{
			m_VertexBufferManager.CreateModelVertexBuffer(*modelMesh.get());
			const auto& existingSubMeshes = GetSubMeshes(modelMesh.get()->GetVertexBufferId());

			if (existingSubMeshes.empty())
			{
				AddMeshes(modelMesh.get()->GetMeshData());
				return;
			}

			std::vector<MeshData> duplicatedMeshes(existingSubMeshes.size());
			std::transform(existingSubMeshes.begin(), existingSubMeshes.end(), duplicatedMeshes.begin(),
				[&](const MeshData& _subMesh)
				{
					MeshData copiedSubMesh{ _subMesh };
					copiedSubMesh.SetEntityId(modelMesh.get()->GetOwner()->GetUniqueId());
					copiedSubMesh.SetShaderType(modelMesh.get()->GetShaderType());

					if (modelMesh.get()->GetTintColor().has_value())
					{
						const glm::vec3& tintColor{ modelMesh.get()->GetTintColor().value() };
						copiedSubMesh.SetDiffuseColor(tintColor);
						copiedSubMesh.SetSpecularColor(tintColor);
					}

					return copiedSubMesh;
				});

			AddMeshes(duplicatedMeshes);
		}
	}

	void MeshSystem::AddMeshes(std::vector<MeshData>& _meshes)
	{
		for (auto& mesh : _meshes)
		{
			AddMesh(mesh);
		}

		m_IsCacheDirty = true;
	}

	void MeshSystem::AddMesh(MeshData& _mesh)
	{
		const uint32 vertexBufferId{ _mesh.GetVertexBufferId() };
		_mesh.SetMeshId(m_TotalMeshCount++);
		m_VertexBufferIdToSubMeshes[vertexBufferId].emplace_back(_mesh);
		m_IsCacheDirty = true;
	}

	const std::vector<MeshData>& MeshSystem::GetAllSubMeshes()
	{
		if (m_IsCacheDirty)
		{
			UpdateSubMeshCache();
		}

		return m_CachedSubMeshes;
	}

	const std::vector<MeshData>& MeshSystem::GetSubMeshes(const uint32 _bufferId) const
	{
		static const std::vector<MeshData> empty{};

		const auto it{ m_VertexBufferIdToSubMeshes.find(_bufferId) };
		if (it != m_VertexBufferIdToSubMeshes.end())
		{
			return it->second;
		}

		return empty;
	}

	VulkanVertexBuffer* MeshSystem::GetVertexBuffer(const uint32 _bufferId)
	{
		return m_VertexBufferManager.GetVertexBuffer(_bufferId);
	}

	void MeshSystem::UpdateSubMeshCache()
	{
		m_CachedSubMeshes.clear();

		for (const auto& mesh : m_VertexBufferIdToSubMeshes)
		{
			const auto& subMeshes{ mesh.second };
			m_CachedSubMeshes.insert(m_CachedSubMeshes.end(), subMeshes.begin(), subMeshes.end());
		}

		m_IsCacheDirty = false;
	}
} // End of Banshee namespace