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
		m_HasCustomTexture = true;
	}

	void MeshComponent::SetSortLayer(const uint32 _layerId, const int32 _priority) noexcept
	{
		m_SortingLayer.layerId = _layerId;
		m_SortingLayer.priority = _priority;
	}

	std::string MeshComponent::GetModelPath() const
	{
		return ResourceManager::Instance().GetEngineResDirPath() + m_ModelPath;
	}
} // End of Banshee namespace