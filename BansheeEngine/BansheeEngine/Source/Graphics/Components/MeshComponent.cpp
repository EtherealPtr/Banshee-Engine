#include "MeshComponent.h"
#include "Foundation/Paths/PathManager.h"
#include "Foundation/ResourceManager/ResourceManager.h"

namespace Banshee
{
	MeshComponent::MeshComponent(std::string_view _modelPath, const ShaderType _shaderType) :
		m_VertexBufferId{ 0 },
		m_ShaderType{ _shaderType },
		m_ModelName{ g_ResourceManager.GetAssetName(_modelPath) },
		m_HasModel{ true }
	{}

	MeshComponent::MeshComponent(const PrimitiveShape _basicShape, const ShaderType _shaderType) :
		m_VertexBufferId{ 0 },
		m_ShaderType{ _shaderType },
		m_ModelName{ "" },
		m_HasModel{ false }
	{}

	const std::string MeshComponent::GetModelPath() const
	{
		const std::string_view modelsFolder{ "Models/" };
		return PathManager::GetEngineResDirPath() + modelsFolder.data() + m_ModelName;
	}
} // End of Banshee namespace
