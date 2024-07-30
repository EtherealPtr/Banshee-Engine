#include "MeshComponent.h"
#include "Foundation/Paths/PathManager.h"
#include "Foundation/ResourceManager/ResourceManager.h"

namespace Banshee
{
	MeshComponent::MeshComponent(const std::string& _modelPath, const ShaderType _shaderType) :
		m_MeshId{ 0 },
		m_TexId{ 0 },
		m_ShaderType{ _shaderType },
		m_Meshes{},
		m_ModelName{ g_ResourceManager.GetAssetName(_modelPath) },
		m_HasModel{ true },
		m_HasTexture{ false },
		m_Color{ glm::vec3{1.0f} }
	{}

	MeshComponent::MeshComponent(const PrimitiveShape _basicShape, const ShaderType _shaderType, const glm::vec3& _color) :
		m_MeshId{ 0 },
		m_TexId{ 0 },
		m_ShaderType{ _shaderType },
		m_Meshes{},
		m_ModelName{ "" },
		m_HasModel{ false },
		m_HasTexture{ false },
		m_Color{ _color }
	{
		SetMeshId(static_cast<uint32>(_basicShape));
	}

	void MeshComponent::SetTexture(const char* _pathToTexture)
	{
		m_TexId = g_ResourceManager.LoadImageResource(_pathToTexture);
		m_HasTexture = true;
	}

	const std::string MeshComponent::GetModelPath() const
	{
		const std::string modelsFolder = "Models/";
		return PathManager::GetEngineResDirPath() + modelsFolder + m_ModelName;
	}
} // End of Banshee namespace
