#include "VulkanVertexBufferManager.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/Components/MeshComponent.h"
#include "Graphics/ShapeFactory.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <stdexcept>

namespace Banshee
{
	const uint32 g_ModelIdOffset = 1000;

	VulkanVertexBufferManager::VulkanVertexBufferManager(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue) :
		m_LogicalDevice(_logicalDevice),
		m_PhysicalDevice(_physicalDevice),
		m_CommandPool(_commandPool),
		m_GraphicsQueue(_graphicsQueue),
		m_VertexBuffers{}
	{}

	void VulkanVertexBufferManager::GenerateBuffers(const uint32 _bufferId, void* _vertexData, const uint64 _sizeOfVertexData, void* _indexData, const uint64 _sizeOfIndexData)
	{
		auto result = m_VertexBuffers.emplace
		(
			std::piecewise_construct,
			std::forward_as_tuple(_bufferId),
			std::forward_as_tuple(m_LogicalDevice, m_PhysicalDevice, m_CommandPool, m_GraphicsQueue, _vertexData, _sizeOfVertexData, _indexData, _sizeOfIndexData)
		);

		if (!result.second)
		{
			BE_LOG(LogCategory::Error, "[VERTEX MANAGER]: Failed to generate vertex buffer with id: %d", _bufferId);
		}
	}

	void VulkanVertexBufferManager::CreateBasicShapeVertexBuffer(MeshComponent* _meshComponent)
	{
		const uint32 bufferId = _meshComponent->GetMeshId();
		auto vertexBuffer = m_VertexBuffers.find(bufferId);

		if (vertexBuffer != m_VertexBuffers.end())
		{
			return;
		}
		else
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32> indices{};

			ShapeFactory::GetShapeData(static_cast<PrimitiveShape>(bufferId), vertices, indices);

			GenerateBuffers(bufferId, vertices.data(), sizeof(Vertex) * vertices.size(), indices.data(), sizeof(uint32) * indices.size());
			_meshComponent->SetIndicesCount(indices.size());
		}
	}

	void VulkanVertexBufferManager::CreateModelVertexBuffer(MeshComponent* _meshComponent)
	{
        const std::string modelPath = _meshComponent->GetModelPath();
        const std::string modelName = _meshComponent->GetModelName();

        uint32 modelId = 0;
        auto it = m_ModelNameToIdMap.find(modelName);

        if (it != m_ModelNameToIdMap.end())
        {
            modelId = it->second;
        }
        else
        {
            modelId = m_ModelNameToIdMap.size();
            modelId += g_ModelIdOffset;
            m_ModelNameToIdMap[modelName] = modelId;
            _meshComponent->SetMeshId(modelId);
        }

        auto vertexBuffer = m_VertexBuffers.find(modelId);
        if (vertexBuffer != m_VertexBuffers.end())
        {
            return;
        }
        else
        {
            Assimp::Importer importer{};
            const aiScene* scene = importer.ReadFile(modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
            if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                BE_LOG(LogCategory::Error, "[ASSIMP]: Model loading error: %s", importer.GetErrorString());
                return;
            }

            std::vector<Vertex> vertices{};
            std::vector<uint32> indices{};

            for (uint32 m = 0; m < scene->mNumMeshes; ++m)
            {
                const aiMesh* const mesh = scene->mMeshes[m];
                const uint32 vertexBase = vertices.size();

                // Load vertices
                for (uint32 i = 0; i < mesh->mNumVertices; ++i)
                {
                    Vertex vertex{mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z, 0.0f, 0.0f };
                    if (mesh->mTextureCoords[0])
                    {
                        vertex.texCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
                    }
                    vertices.push_back(vertex);
                }

                // Load indices
                for (uint32 i = 0; i < mesh->mNumFaces; ++i)
                {
                    aiFace face = mesh->mFaces[i];
                    for (uint32 j = 0; j < face.mNumIndices; ++j)
                    {
                        indices.push_back(face.mIndices[j] + vertexBase);
                    }
                }
            }

            std::vector<Material> materials{};
            if (scene->HasMaterials())
            {
                materials.reserve(scene->mNumMaterials);
                
                for (uint32 m = 0; m < scene->mNumMaterials; ++m)
                {
                    const aiMaterial* const material = scene->mMaterials[m];
                    Material newMaterial{};

                    // Diffuse color
                    aiColor3D color(0.0f, 0.0f, 0.0f);
                    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_DIFFUSE, color))
                    {
                        newMaterial.SetDiffuseColor({ color.r, color.g, color.b });
                    }

                    // Specular color
                    if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_SPECULAR, color))
                    {
                        newMaterial.SetSpecularColor({ color.r, color.g, color.b });
                    }

                    // Shininess
                    float shininess = 0.0f;
                    if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, shininess))
                    {
                        newMaterial.SetShininess(shininess);
                    }

                    // Diffuse texture
                    if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0)
                    {
                        aiString texturePath{};
                        if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texturePath) == AI_SUCCESS)
                        {
                            const std::string textureFolderName = "Textures/";
                            const std::string textureName = textureFolderName + ResourceManager::Instance().GetAssetName(texturePath.C_Str());
                            _meshComponent->SetTexture(textureName.c_str());
                        }
                    }

                    materials.push_back(newMaterial);
                }
            }

            _meshComponent->SetMaterials(materials);

            GenerateBuffers(modelId, vertices.data(), sizeof(Vertex) * vertices.size(), indices.data(), sizeof(uint32) * indices.size());
            _meshComponent->SetIndicesCount(indices.size());
        }
	}

	VulkanVertexBuffer* VulkanVertexBufferManager::GetVertexBuffer(const uint32 _meshId)
	{
		auto vertexBuffer = m_VertexBuffers.find(_meshId);
		if (vertexBuffer != m_VertexBuffers.end())
		{
			return &vertexBuffer->second;
		}
		else
		{
			BE_LOG(LogCategory::Error, "[VERTEX MANAGER]: Failed to find vertex buffer with id: %d", _meshId);
			throw std::runtime_error("Mesh id not found in vertex buffers map");
		}
	}
} // End of Banshee namespace