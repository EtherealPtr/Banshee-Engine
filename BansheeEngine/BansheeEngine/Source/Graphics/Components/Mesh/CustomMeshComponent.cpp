#include "CustomMeshComponent.h"
#include "Foundation/Entity/Entity.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/Paths/PathManager.h"

namespace Banshee
{
	CustomMeshComponent::CustomMeshComponent(const std::string_view _modelPath, const ShaderType _shaderType) : 
		MeshComponent(_shaderType),
		m_ModelName{ g_ResourceManager.GetAssetName(_modelPath) },
		m_Data{}
	{}

	void CustomMeshComponent::AddMeshData(MeshData& _data)
	{
		_data.SetVertexBufferId(GetVertexBufferId());
		_data.SetEntityId(GetOwner()->GetUniqueId());
		_data.SetShaderType(GetShaderType());
		m_Data.emplace_back(_data);
	}

	const std::string CustomMeshComponent::GetModelPath() const
	{
		const std::string_view modelsFolder{ "Models/" };
		return PathManager::GetEngineResDirPath() + modelsFolder.data() + m_ModelName;
	}
} // End of Banshee namespace