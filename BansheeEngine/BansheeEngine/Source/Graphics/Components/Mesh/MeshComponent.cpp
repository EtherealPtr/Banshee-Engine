#include "MeshComponent.h"

namespace Banshee
{
	MeshComponent::MeshComponent(const ShaderType _shaderType) noexcept :
		m_VertexBufferId{ 0 },
		m_ShaderType{ _shaderType }
	{}
} // End of Banshee namespace