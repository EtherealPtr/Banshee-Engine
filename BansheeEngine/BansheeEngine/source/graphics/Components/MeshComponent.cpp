#include "MeshComponent.h"
#include "Foundation/ResourceManager/ResourceManager.h"

namespace Banshee
{
	MeshComponent::MeshComponent() :
		m_MeshId(0),
		m_TexId(0),
		m_Vertices(),
		m_Indices(),
		m_Meshes(),
		m_ModelName(""),
		m_HasModel(false),
		m_HasTexture(false)
	{
		m_Material = std::make_shared<Material>();
	}

	void MeshComponent::SetModel(const char* _modelName)
	{
		m_ModelName = _modelName;
		m_HasModel = true;
	}

	void MeshComponent::SetCombinedMeshData(const std::vector<Vertex>& _vertices, const std::vector<uint32>& _indices)
	{
		m_Vertices = _vertices;
		m_Indices = _indices;
	}

	void MeshComponent::SetTexture(const char* _pathToTexture) noexcept
	{
		m_TexId = ResourceManager::Instance().LoadImageResource(_pathToTexture);
		m_HasTexture = true;
	}

	const std::string MeshComponent::GetModelPath() const
	{
		return ResourceManager::Instance().GetEngineResDirPath() + m_ModelName;
	}
} // End of Banshee namespace
