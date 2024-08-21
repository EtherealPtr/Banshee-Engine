#pragma once

#include "Foundation/Platform.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace Banshee
{
	class Mesh;

	class MeshSystem
	{
	public:
		MeshSystem() noexcept;
		~MeshSystem() noexcept = default;

		void AddMeshes(const std::vector<Mesh>& _meshes);
		void AddMesh(const Mesh& _mesh);
		const std::vector<Mesh>& GetAllSubMeshes();
		const std::vector<Mesh>& GetSubMeshes(const uint32 _bufferId) const;

		MeshSystem(const MeshSystem&) = delete;
		MeshSystem(MeshSystem&&) = delete;
		MeshSystem& operator=(const MeshSystem&) = delete;
		MeshSystem& operator=(MeshSystem&&) = delete;

	private:
		void UpdateSubMeshCache();

	private:
		std::vector<Mesh> m_CachedSubMeshes;
		std::unordered_map<uint32, std::vector<Mesh>> m_VertexBufferIdToSubMeshes;
		bool m_IsCacheDirty;
	};
} // End of Banshee namespace