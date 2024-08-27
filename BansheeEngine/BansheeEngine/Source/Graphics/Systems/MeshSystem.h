#pragma once

#include "Foundation/Platform.h"
#include <memory>
#include <vector>
#include <unordered_map>

namespace Banshee
{
	class MeshData;

	class MeshSystem
	{
	public:
		MeshSystem() noexcept;
		~MeshSystem() noexcept = default;

		void AddMeshes(const std::vector<MeshData>& _meshes);
		void AddMesh(const MeshData& _mesh);
		const std::vector<MeshData>& GetAllSubMeshes();
		const std::vector<MeshData>& GetSubMeshes(const uint32 _bufferId) const;

		MeshSystem(const MeshSystem&) = delete;
		MeshSystem(MeshSystem&&) = delete;
		MeshSystem& operator=(const MeshSystem&) = delete;
		MeshSystem& operator=(MeshSystem&&) = delete;

	private:
		void UpdateSubMeshCache();

	private:
		std::vector<MeshData> m_CachedSubMeshes;
		std::unordered_map<uint32, std::vector<MeshData>> m_VertexBufferIdToSubMeshes;
		bool m_IsCacheDirty;
	};
} // End of Banshee namespace