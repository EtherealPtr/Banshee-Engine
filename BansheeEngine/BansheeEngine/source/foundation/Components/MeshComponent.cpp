#include "MeshComponent.h"
#include "Foundation/ResourceManager/ResourceManager.h"

namespace Banshee
{
	void MeshComponent::SetModel(const char* _modelName)
	{
		m_ModelName = _modelName;
		m_HasModel = true;
	}

	void MeshComponent::SetTexture(const char* _pathToTexture)
	{
		m_TexId = m_Materials[0].SetTexture(_pathToTexture);
		m_HasTexture = true;
	}

	const std::string MeshComponent::GetModelPath() const
	{
		return ResourceManager::Instance().GetEngineResDirPath() + m_ModelName;
	}
} // End of Banshee namespace
