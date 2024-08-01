#include "MeshComponent.h"
#include "Foundation/Paths/PathManager.h"
#include "Foundation/ResourceManager/ResourceManager.h"

namespace Banshee
{
	MeshComponent::MeshComponent(std::string_view _modelPath, const ShaderType _shaderType) :
		m_MeshId{ 0 },
		m_TexId{ 0 },
		m_ShaderType{ _shaderType },
		m_Meshes{},
		m_ModelName{ g_ResourceManager.GetAssetName(_modelPath) },
		m_Color{ glm::vec3{1.0f} },
		m_HasModel{ true },
		m_HasTexture{ false }
	{}

	MeshComponent::MeshComponent(const PrimitiveShape _basicShape, const ShaderType _shaderType, const glm::vec3& _color) :
		m_MeshId{ 0 },
		m_TexId{ 0 },
		m_ShaderType{ _shaderType },
		m_Meshes{},
		m_ModelName{ "" },
		m_Color{ _color },
		m_HasModel{ false },
		m_HasTexture{ false }
	{
		SetMeshId(static_cast<uint32>(_basicShape));
	}

	void MeshComponent::SetTexture(std::string_view _pathToTexture)
	{
		m_TexId = g_ResourceManager.LoadImageResource(_pathToTexture.data());
		m_HasTexture = true;
	}

	const std::string MeshComponent::GetModelPath() const
	{
		const std::string_view modelsFolder{ "Models/" };
		return PathManager::GetEngineResDirPath() + modelsFolder.data() + m_ModelName;
	}
} // End of Banshee namespace
