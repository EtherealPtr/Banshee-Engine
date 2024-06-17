#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/DLLConfig.h"
#include "Mesh.h"
#include <string>

namespace Banshee
{
	class MeshComponent : public Component
	{
	public:
		BANSHEE_ENGINE MeshComponent();

		BANSHEE_ENGINE void SetModel(const char* _modelName);
		BANSHEE_ENGINE void SetTexture(const char* _pathToTexture) noexcept;
		void SetSubMeshData(const std::vector<Mesh>& _meshData) { m_Meshes = _meshData; }
		void SetCombinedMeshData(const std::vector<Vertex>& vertices, const std::vector<uint32>& indices);
		void SetMeshId(uint32 _meshId) noexcept { m_MeshId = _meshId; }
		void SetSubMesh(const Mesh& _subMesh) { m_Meshes.push_back(_subMesh); }

		uint32 GetMeshId() const noexcept { return m_MeshId; }
		uint16 GetTexId() const noexcept { return m_TexId; }
		const std::vector<Mesh>& GetSubMeshes() noexcept { return m_Meshes; }
		const std::string& GetModelName() const noexcept { return m_ModelName; }
		const std::vector<Vertex>& GetVertices() noexcept { return m_Vertices; }
		const std::vector<uint32>& GetIndices() noexcept { return m_Indices; }
		bool HasTexture() const noexcept { return m_HasTexture; }
		bool HasModel() const noexcept { return m_HasModel; }
		const std::string GetModelPath() const;
		const std::shared_ptr<Material>& GetMaterial() const noexcept { return m_Material; }

	private:
		uint32 m_MeshId;
		uint16 m_TexId;
		std::vector<Vertex> m_Vertices;
		std::vector<uint32> m_Indices;
		std::vector<Mesh> m_Meshes;
		std::string m_ModelName;
		bool m_HasTexture;
		bool m_HasModel;
		std::shared_ptr<Material> m_Material;
	};
} // End of Banshee namespace