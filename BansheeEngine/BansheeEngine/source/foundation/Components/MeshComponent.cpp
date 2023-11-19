#include "MeshComponent.h"
#include "Foundation/ResourceManager/ResourceManager.h"

namespace Banshee
{
	void MeshComponent::SetColor(const float _r, const float _g, const float _b) noexcept
	{
		m_Color[0] = _r;
		m_Color[1] = _g;
		m_Color[2] = _b;
	}

	void MeshComponent::SetTexture(const char* _pathToTexture)
	{
		m_TexId = ResourceManager::Instance().LoadImageResource(_pathToTexture);
	}
} // End of Banshee namespace