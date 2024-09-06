#pragma once

#include "Foundation/Platform.h"
#include "Material.h"
#include <vector>
#include "Foundation/Logging/Logger.h"

namespace Banshee
{
	class MeshData
	{
	public:
		MeshData() noexcept :
			m_EntityId{ 0 },
			m_VertexBufferId{ 0 },
			m_IndexOffset{ 0 },
			m_MaterialIndex{ SetNextMaterialIndex() },
			m_TexId{ 0 },
			m_Indices{},
			m_Material{},
			m_Model{ 1.0f }
		{}
		~MeshData() = default;
		MeshData(const MeshData& _other) : 
			m_EntityId(_other.m_EntityId),                
			m_VertexBufferId(_other.m_VertexBufferId),    
			m_MaterialIndex(SetNextMaterialIndex()),      
			m_IndexOffset(_other.m_IndexOffset),          
			m_TexId(_other.m_TexId),                      
			m_Indices(_other.m_Indices),                  
			m_Material(_other.m_Material),                
			m_Model(_other.m_Model)
		{}
		MeshData& operator=(const MeshData& _other)
		{
			m_EntityId = _other.m_EntityId;
			m_VertexBufferId = _other.m_VertexBufferId;
			m_MaterialIndex = SetNextMaterialIndex();  
			m_IndexOffset = _other.m_IndexOffset;
			m_TexId = _other.m_TexId;
			m_Indices = _other.m_Indices;
			m_Material = _other.m_Material;
			m_Model = _other.m_Model;
			BE_LOG(LogCategory::Warning, "MeshData copy ctor");
			return *this;
		}
		MeshData(MeshData&& _other) noexcept : 
			m_EntityId(_other.m_EntityId),
			m_VertexBufferId(_other.m_VertexBufferId),
			m_MaterialIndex(SetNextMaterialIndex()), 
			m_IndexOffset(_other.m_IndexOffset),
			m_TexId(_other.m_TexId),
			m_Indices(std::move(_other.m_Indices)), 
			m_Material(std::move(_other.m_Material)),
			m_Model(std::move(_other.m_Model))        
		{
			BE_LOG(LogCategory::Warning, "MeshData move ctor");
		}
		MeshData& operator=(MeshData&& _other) noexcept
		{
			m_EntityId = _other.m_EntityId;
			m_VertexBufferId = _other.m_VertexBufferId;
			m_MaterialIndex = SetNextMaterialIndex();
			m_IndexOffset = _other.m_IndexOffset;
			m_TexId = _other.m_TexId;
			m_Indices = std::move(_other.m_Indices);
			m_Material = std::move(_other.m_Material);
			m_Model = std::move(_other.m_Model);
			BE_LOG(LogCategory::Warning, "MeshData move assignment ctor");

			return *this;
		}

		void SetEntityId(const uint32 _entityId) noexcept { m_EntityId = _entityId; }
		void SetTexId(const uint16 _texId) noexcept { m_TexId = _texId; }
		void SetVertexBufferId(const uint32 _vertexBufferId) noexcept { m_VertexBufferId = _vertexBufferId; }
		void SetIndexOffset(const uint32 _indexOffset) noexcept { m_IndexOffset = _indexOffset; }
		void SetIndices(const std::vector<uint32>& _indices) noexcept { m_Indices = _indices; }
		void SetModelMatrix(const glm::mat4& _modelMatrix) noexcept { m_Model = _modelMatrix; }
		void SetDiffuseColor(const glm::vec3& _col) noexcept { m_Material.SetDiffuseColor(_col); }
		void SetSpecularColor(const glm::vec3& _col) noexcept { m_Material.SetSpecularColor(_col); }
		void SetShaderType(const ShaderType _shaderType) noexcept { m_Material.SetShaderType(_shaderType); }

		uint32 GetEntityId() const noexcept { return m_EntityId; }
		uint32 GetMaterialIndex() const noexcept { return m_MaterialIndex; }
		uint32 GetVertexBufferId() const noexcept { return m_VertexBufferId; }
		uint32 GetIndexOffset() const noexcept { return m_IndexOffset; }
		uint16 GetTexId() const noexcept { return m_TexId; }
		const std::vector<uint32>& GetIndices() const noexcept { return m_Indices; }
		const Material& GetMaterial() const noexcept { return m_Material; }
		const glm::mat4& GetModelMatrix() const noexcept { return m_Model; }

	private:
		static uint32 SetNextMaterialIndex() noexcept
		{
			static uint32 nextIndex{ 0 };
			return ++nextIndex;
		}

	private:
		uint32 m_EntityId;
		uint32 m_VertexBufferId;
		uint32 m_MaterialIndex;
		uint32 m_IndexOffset;   // Offset into the index buffer
		uint16 m_TexId;
		std::vector<uint32> m_Indices{};
		Material m_Material;
		glm::mat4 m_Model;
	};
} // End of Banshee namespace