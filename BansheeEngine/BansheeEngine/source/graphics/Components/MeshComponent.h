#pragma once

#include "Foundation/Components/Component.h"
#include "Foundation/DLLConfig.h"
#include "Mesh.h"
#include "Graphics/PrimitiveShape.h"
#include <string>

namespace Banshee
{
	class MeshComponent : public Component
	{
	public:
		BANSHEE_ENGINE MeshComponent(const std::string& _modelPath);
		BANSHEE_ENGINE MeshComponent(const PrimitiveShape _basicShape, const glm::vec3& _color = glm::vec3(1.0f));

		BANSHEE_ENGINE void SetTexture(const char* _pathToTexture) noexcept;
		void SetMeshId(const uint32 _meshId) noexcept { m_MeshId = _meshId; }
		void SetModel(const char* _modelName);
		void SetSubMesh(const Mesh& _subMesh) { m_Meshes.push_back(_subMesh); }
		void SetSubMeshes(const std::vector<Mesh>& subMeshes) { m_Meshes = subMeshes; }
		void SetCombinedMeshData(const std::vector<Vertex>& _vertices, const std::vector<uint32>& _indices);

		uint32 GetMeshId() const noexcept { return m_MeshId; }
		uint16 GetTexId() const noexcept { return m_TexId; }
		const std::vector<Mesh>& GetSubMeshes() noexcept { return m_Meshes; }
		const std::string& GetModelName() const noexcept { return m_ModelName; }
		const std::vector<Vertex>& GetVertices() noexcept { return m_Vertices; }
		const std::vector<uint32>& GetIndices() noexcept { return m_Indices; }
		const std::string GetModelPath() const;
		const glm::vec3& GetColor() const noexcept { return m_Color; }
		bool HasTexture() const noexcept { return m_HasTexture; }
		bool HasModel() const noexcept { return m_HasModel; }

	private:
		uint32 m_MeshId;
		uint16 m_TexId;
		std::vector<Vertex> m_Vertices;
		std::vector<uint32> m_Indices;
		std::vector<Mesh> m_Meshes;
		std::string m_ModelName;
		bool m_HasTexture;
		bool m_HasModel;
		glm::vec3 m_Color;
	};
} // End of Banshee namespace