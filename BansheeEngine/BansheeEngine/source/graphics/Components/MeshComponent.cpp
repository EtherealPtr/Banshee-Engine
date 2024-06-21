#include "MeshComponent.h"
#include "Foundation/ResourceManager/ResourceManager.h"

namespace Banshee
{
	MeshComponent::MeshComponent(const std::string& _modelPath) :
		m_MeshId(0),
		m_TexId(0),
		m_Vertices(),
		m_Indices(),
		m_Meshes(),
		m_ModelName(""),
		m_HasModel(false),
		m_HasTexture(false),
		m_Color(glm::vec3(1.0f))
	{
		SetModel(_modelPath.c_str());
	}

	MeshComponent::MeshComponent(const PrimitiveShape _basicShape, const glm::vec3& _color) :
		m_MeshId(0),
		m_TexId(0),
		m_Vertices(),
		m_Indices(),
		m_Meshes(),
		m_ModelName(""),
		m_HasModel(false),
		m_HasTexture(false),
		m_Color(_color)
	{
		SetMeshId(static_cast<uint32>(_basicShape));
	}

	void MeshComponent::SetModel(const char* _modelName)
	{
		m_ModelName = ResourceManager::Instance().GetAssetName(_modelName);
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
		const std::string modelsFolder = "Models/";
		return ResourceManager::Instance().GetEngineResDirPath() + modelsFolder + m_ModelName;
	}
} // End of Banshee namespace
