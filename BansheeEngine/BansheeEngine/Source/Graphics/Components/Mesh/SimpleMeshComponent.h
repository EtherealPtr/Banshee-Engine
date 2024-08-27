#pragma once

#include "MeshComponent.h"
#include "Graphics/PrimitiveShape.h"
#include "Graphics/MeshData.h"

namespace Banshee
{
	class SimpleMeshComponent : public MeshComponent
	{
	public:
		BANSHEE_ENGINE SimpleMeshComponent(const PrimitiveShape _basicShape, const ShaderType _shaderType = ShaderType::Standard, const glm::vec3& _col = glm::vec3(1.0f)) noexcept;

		BANSHEE_ENGINE void SetTexture(const std::string_view _texturePath);
		void SetIndices(const std::vector<uint32>& _indices) noexcept { m_Data.SetIndices(_indices); }
		PrimitiveShape GetShape() const noexcept { return m_Shape; }
		MeshData& GetMeshData() noexcept { return m_Data; }

	private:
		PrimitiveShape m_Shape;
		MeshData m_Data;
	};
} // End of Banshee namespace
