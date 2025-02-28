#include "PrimitiveMeshComponent.h"
#include "Foundation/Entity/Entity.h"
#include "Foundation/ResourceManager/ResourceManager.h"

namespace Banshee
{
	PrimitiveMeshComponent::PrimitiveMeshComponent(const PrimitiveShapeEnum _shape, const ShaderTypeEnum _shaderType) noexcept :
		MeshComponent(_shaderType),
		m_Shape{ _shape },
		m_Data{}
	{
		m_Data.SetShaderType(_shaderType);
	}

	void PrimitiveMeshComponent::OnComponentInitialized() noexcept
	{
		m_Data.SetEntityId(GetOwner()->GetUniqueId());
	}

	void PrimitiveMeshComponent::SetTintColor(const glm::vec4& _tintColor) noexcept
	{
		MeshComponent::SetTintColor(_tintColor);

		m_Data.SetDiffuseColor(_tintColor);
	}

	void PrimitiveMeshComponent::SetVertexBufferId(const uint32 _vertexBufferId) noexcept
	{
		MeshComponent::SetVertexBufferId(_vertexBufferId);

		m_Data.SetVertexBufferId(_vertexBufferId);
	}

	void PrimitiveMeshComponent::SetTexture(std::string_view _texturePath)
	{
		m_Data.SetTexId(g_ResourceManager.LoadImageResource(_texturePath));
	}
} // End of namespace