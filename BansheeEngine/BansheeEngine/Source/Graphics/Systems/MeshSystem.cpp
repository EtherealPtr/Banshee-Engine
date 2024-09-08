#include "MeshSystem.h"
#include "Graphics/MeshData.h"

namespace Banshee
{
	MeshSystem::MeshSystem() noexcept :
		m_VertexBufferIdToSubMeshes{},
		m_CachedSubMeshes{},
		m_IsCacheDirty{ true },
		m_TotalMeshCount{ 0 }
	{}

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