#include "SimpleMeshComponent.h"
#include "Foundation/Entity/Entity.h"
#include "Foundation/ResourceManager/ResourceManager.h"

namespace Banshee
{
	SimpleMeshComponent::SimpleMeshComponent(const PrimitiveShape _shape, const ShaderType _shaderType, const glm::vec3& _col) noexcept :
		MeshComponent(_shaderType),
		m_Shape{ _shape },
		m_Data{}
	{
		m_Data.SetShaderType(_shaderType);
	}

	void SimpleMeshComponent::SetTexture(const std::string_view _texturePath)
	{
		m_Data.SetTexId(g_ResourceManager.LoadImageResource(_texturePath));
	}
} // End of Banshee namespace