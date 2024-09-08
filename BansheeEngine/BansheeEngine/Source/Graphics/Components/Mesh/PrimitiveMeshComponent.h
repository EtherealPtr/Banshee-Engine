#pragma once

#include "MeshComponent.h"
#include "Graphics/PrimitiveShape.h"
#include "Graphics/MeshData.h"

namespace Banshee
{
	class PrimitiveMeshComponent : public MeshComponent
	{
	public:
		BANSHEE_ENGINE PrimitiveMeshComponent(const PrimitiveShape _basicShape, const ShaderType _shaderType = ShaderType::Standard) noexcept;

		virtual void OnComponentInitialized() noexcept override;
		virtual void SetTintColor(const glm::vec3& _tintColor) noexcept override;
		BANSHEE_ENGINE void SetTexture(const std::string_view _texturePath);
		void SetIndices(const uint32 _indices) noexcept { m_Data.SetIndexCount(_indices); }
		PrimitiveShape GetShape() const noexcept { return m_Shape; }
		MeshData& GetMeshData() noexcept { return m_Data; }

	private:
		PrimitiveShape m_Shape;
		MeshData m_Data;
	};
} // End of Banshee namespace