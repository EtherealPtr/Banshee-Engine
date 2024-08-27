#include "MeshComponent.h"

namespace Banshee
{
	MeshComponent::MeshComponent(const ShaderType _shaderType) noexcept :
		m_VertexBufferId{ 0 },
		m_ShaderType{ _shaderType },
		m_TintColor{ glm::vec3(1.0f) }
	{}
} // End of Banshee namespace
