#pragma once

#include "Component.h"
#include "Material.h"
#include "Foundation/DLLConfig.h"
#include <vector>

namespace Banshee
{
	class Material;

	class MeshComponent : public Component
	{
	public:
		BANSHEE_ENGINE MeshComponent(const uint32 _meshId = 0) :
			m_MeshId(_meshId),
			m_IndicesCount(0),
			m_HasTexture(false),
			m_HasModel(false),
			m_ModelName(""),
			m_TexId(0)
		{
			m_Materials.resize(1);
		}

		BANSHEE_ENGINE void SetModel(const char* _modelName);
		BANSHEE_ENGINE void SetTexture(const char* _pathToTexture);
		void SetMeshId(const uint32 _meshId) noexcept { m_MeshId = _meshId; }
		void SetIndicesCount(const uint32 _count) noexcept { m_IndicesCount = _count; }
		void SetMaterials(const std::vector<Material>& _materials) { m_Materials = _materials; }

		uint32 GetMeshId() const noexcept { return m_MeshId; }
		uint32 GetIndicesCount() const noexcept { return m_IndicesCount; }
		uint16 GetTexId() const noexcept { return m_TexId; }
		std::vector<Material>& GetMaterials() noexcept { return m_Materials; }
		const std::string& GetModelName() const noexcept { return m_ModelName; }
		const std::string GetModelPath() const;
		bool HasTexture() const noexcept { return m_HasTexture; }
		bool HasModel() const noexcept { return m_HasModel; }

	private:
		uint32 m_MeshId;
		uint32 m_IndicesCount;
		std::vector<Material> m_Materials;
		bool m_HasTexture;
		bool m_HasModel;
		std::string m_ModelName;
		uint16 m_TexId;
	};
} // End of Banshee namespace