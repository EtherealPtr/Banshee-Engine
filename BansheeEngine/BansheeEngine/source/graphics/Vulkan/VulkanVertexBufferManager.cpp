#include "VulkanVertexBufferManager.h"
#include "Foundation/Logging/Logger.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Graphics/ShapeFactory.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Banshee
{
	const uint32 g_ModelIdOffset = 1000;

	VulkanVertexBufferManager::VulkanVertexBufferManager(const VkDevice& _logicalDevice, const VkPhysicalDevice& _physicalDevice, const VkCommandPool& _commandPool, const VkQueue& _graphicsQueue) :
		m_LogicalDevice(_logicalDevice),
		m_PhysicalDevice(_physicalDevice),
		m_CommandPool(_commandPool),
		m_GraphicsQueue(_graphicsQueue),
		m_CurrentIndicesCount(0),
		m_VertexBuffers{}
	{}

	VulkanVertexBuffer& VulkanVertexBufferManager::GenerateBuffers(const uint32 _bufferId, void* _vertexData, const uint64 _sizeOfVertexData, void* _indexData, const uint64 _sizeOfIndexData)
	{
		auto result = m_VertexBuffers.emplace
		(
			std::piecewise_construct,
			std::forward_as_tuple(_bufferId),
			std::forward_as_tuple(m_LogicalDevice, m_PhysicalDevice, m_CommandPool, m_GraphicsQueue, _vertexData, _sizeOfVertexData, _indexData, _sizeOfIndexData)
		);

		if (!result.second)
		{
			BE_LOG(LogCategory::Error, "Failed to generate vertex buffer with id: %d", _bufferId);
		}

		return result.first->second;
	}

	void VulkanVertexBufferManager::BindBasicShape(const uint32 _bufferId, const VkCommandBuffer& _commandBuffer)
	{
		auto vertexBuffer = m_VertexBuffers.find(_bufferId);
		if (vertexBuffer != m_VertexBuffers.end())
		{
			m_CurrentIndicesCount = ShapeFactory::GetIndicesCount(static_cast<PrimitiveShape>(_bufferId));
			vertexBuffer->second.Bind(_commandBuffer);
		}
		else
		{
			std::vector<Vertex> vertices{};
			std::vector<uint16> indices{};

			ShapeFactory::GetVertices(static_cast<PrimitiveShape>(_bufferId), vertices, indices);
			VulkanVertexBuffer& newVertexBuffer = GenerateBuffers(_bufferId, vertices.data(), sizeof(Vertex) * vertices.size(), indices.data(), sizeof(uint16) * indices.size());
			newVertexBuffer.SetIndicesCount(static_cast<uint32>(indices.size()));
			BindBasicShape(_bufferId, _commandBuffer);
		}
	}

	void VulkanVertexBufferManager::BindModel(const VkCommandBuffer& _commandBuffer, const std::string& _modelPath)
	{
		const std::string modelName = ResourceManager::Instance().GetAssetName(_modelPath);

		uint32 modelId = 0;
		auto it = m_ModelNameToIdMap.find(modelName);
		if (it != m_ModelNameToIdMap.end())
		{
			modelId = it->second;
		}
		else
		{
			modelId = static_cast<uint32>(m_ModelNameToIdMap.size());
			modelId += g_ModelIdOffset;
			m_ModelNameToIdMap[modelName] = modelId;
		}

		auto vertexBuffer = m_VertexBuffers.find(modelId);
		if (vertexBuffer != m_VertexBuffers.end())
		{
			m_CurrentIndicesCount = vertexBuffer->second.GetIndicesCount();
			vertexBuffer->second.Bind(_commandBuffer);
		}
		else
		{
			Assimp::Importer importer;
			const aiScene* scene = importer.ReadFile(_modelPath, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
			if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
			{
				BE_LOG(LogCategory::Error, "Model loading error: %s", importer.GetErrorString());
				return;
			}

			std::vector<Vertex> vertices{};
			std::vector<uint16> indices{};

			for (uint32 i = 0; i < scene->mMeshes[0]->mNumVertices; ++i)
			{
				Vertex vertex{};
				vertex.position = { scene->mMeshes[0]->mVertices[i].x, scene->mMeshes[0]->mVertices[i].y, scene->mMeshes[0]->mVertices[i].z };
				vertex.texCoord = { scene->mMeshes[0]->mTextureCoords[0][i].x, scene->mMeshes[0]->mTextureCoords[0][i].y };
				//vertex.normal = { scene->mMeshes[0]->mNormals[i].x, scene->mMeshes[0]->mNormals[i].y, scene->mMeshes[0]->mNormals[i].z };
				vertices.push_back(vertex);
			}

			for (uint32 i = 0; i < scene->mMeshes[0]->mNumFaces; ++i)
			{
				aiFace face = scene->mMeshes[0]->mFaces[i];
				for (uint32 j = 0; j < face.mNumIndices; ++j)
				{
					indices.push_back(face.mIndices[j]);
				}
			}

			VulkanVertexBuffer& newVertexBuffer = GenerateBuffers(modelId, vertices.data(), sizeof(Vertex) * vertices.size(), indices.data(), sizeof(uint16) * indices.size());
			newVertexBuffer.SetIndicesCount(static_cast<uint32>(indices.size()));
			BindModel(_commandBuffer, _modelPath);
		}
	}
} // End of Banshee namespace