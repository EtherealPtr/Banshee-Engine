#pragma once

#include "Foundation/Platform.h"
#include "Graphics/Vertex.h"
#include "Graphics/Components/MeshComponent.h"
#include "Material.h"
#include <vector>

namespace Banshee
{
	class Mesh : public MeshComponent
	{
	public:
		Mesh(std::string_view modelPath, const ShaderType shaderType = ShaderType::Standard) :
			MeshComponent{ modelPath, shaderType },
			m_ParentBufferId{ 0 },
			m_MaterialIndex{ SetNextMaterialIndex() },
			m_IndexOffset{ 0 },
			m_TexId{ 0 },
			m_HasTexture{ false },
			m_Vertices{},
			m_Indices{},
			m_Material{},
			m_LocalTransform{ 1.0f }
		{}

		~Mesh() = default;

		void SetTexId(const uint16 _texId) noexcept
		{
			m_TexId = _texId;
			m_HasTexture = true;
		}

		void SetMaterialIndex(const uint32 _materialIndex) noexcept { m_MaterialIndex = _materialIndex; }
		void SetParentVertexBufferId(const uint32 _parentBufferId) noexcept { m_ParentBufferId = _parentBufferId; }
		void SetIndexOffset(const uint32 _indexOffset) noexcept { m_IndexOffset = _indexOffset; }
		void SetVertices(const std::vector<Vertex>& _vertices) noexcept { m_Vertices = _vertices; }
		void SetIndices(const std::vector<uint32>& _indices) noexcept { m_Indices = _indices; }
		void SetMaterial(const Material& _material) noexcept { m_Material = _material; }
		void SetLocalTransform(const glm::mat4& _localTransform) noexcept { m_LocalTransform = _localTransform; }

		bool HasTexture() const noexcept { return m_HasTexture; }
		uint16 GetTexId() const noexcept { return m_TexId; }
		uint32 GetMaterialIndex() const noexcept { return m_MaterialIndex; }
		uint32 GetParentBufferId() const noexcept { return m_ParentBufferId; }
		uint32 GetIndexOffset() const noexcept { return m_IndexOffset; }
		const std::vector<Vertex>& GetVertices() const noexcept { return m_Vertices; }
		const std::vector<uint32>& GetIndices() const noexcept { return m_Indices; }
		const Material& GetMaterial() const noexcept { return m_Material; }
		const glm::mat4& GetLocalTransform() const noexcept { return m_LocalTransform; }

	private:
		static uint32 SetNextMaterialIndex() noexcept
		{
			static uint32 nextIndex{ 0 };
			return ++nextIndex;
		}

	private:
		uint32 m_ParentBufferId;
		uint32 m_MaterialIndex;
		uint32 m_IndexOffset;   // Offset into the index buffer
		uint16 m_TexId;
		bool m_HasTexture;
		std::vector<Vertex> m_Vertices{};
		std::vector<uint32> m_Indices{};
		Material m_Material;
		glm::mat4 m_LocalTransform;
	};
} // End of Banshee namespace