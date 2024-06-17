#include "ModelLoadingSystem.h"
#include "Foundation/Logging/Logger.h"
#include "Graphics/Vertex.h"
#include "Graphics/Components/MeshComponent.h"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"

#define TINYGLTF_NO_STB_IMAGE
#define TINYGLTF_NO_STB_IMAGE_WRITE
#define TINYGLTF_IMPLEMENTATION
#include <tiny_gltf.h>

namespace Banshee
{
	ModelLoadingSystem::ModelLoadingSystem(const char* _modelPath, MeshComponent* _meshComponent)
	{
		assert(_meshComponent != NULL);

		tinygltf::Model model{};
		tinygltf::TinyGLTF loader{};
		std::string err{};
		std::string warn{};

		const bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, _modelPath);
		if (!ret)
		{
			BE_LOG(LogCategory::Error, "[MODEL LOADING SYSTEM]: Failed to load model: %s", err.c_str());
			throw std::runtime_error("Failed to load model");
		}

		LoadModel(model, _meshComponent);
	}

	void ModelLoadingSystem::LoadModel(const tinygltf::Model& _model, MeshComponent* _meshComponent)
	{
		assert(_meshComponent != NULL);

		std::vector<Vertex> combinedVertices{};
		std::vector<uint32> combinedIndices{};

		for (size_t i = 0; i < _model.nodes.size(); ++i)
		{
			const tinygltf::Node& node = _model.nodes[i];
			if (node.mesh < 0)
			{
				continue;
			}

			glm::mat4 nodeTransform { 1.0f };
			GetNodeTransform(node, nodeTransform);

			const tinygltf::Mesh& mesh = _model.meshes[node.mesh];
			for (const auto& primitive : mesh.primitives) 
			{
				Mesh subMesh{};
				subMesh.vertexOffset = static_cast<uint32>(combinedVertices.size());
				subMesh.indexOffset = static_cast<uint32>(combinedIndices.size());

				// Load vertex data
				const auto& positionsAccessor = _model.accessors[primitive.attributes.find("POSITION")->second];
				const auto& textureAccessor = _model.accessors[primitive.attributes.find("TEXCOORD_0")->second];
				const auto& positionsBufferView = _model.bufferViews[positionsAccessor.bufferView];
				const auto& texCoordsBufferView = _model.bufferViews[textureAccessor.bufferView];
				const auto& positionsBuffer = _model.buffers[positionsBufferView.buffer];
				const auto& texCoordsBuffer = _model.buffers[texCoordsBufferView.buffer];
				const float* positions = reinterpret_cast<const float*>(&(positionsBuffer.data[positionsBufferView.byteOffset + positionsAccessor.byteOffset]));
				const float* texCoords = reinterpret_cast<const float*>(&(texCoordsBuffer.data[texCoordsBufferView.byteOffset + textureAccessor.byteOffset]));

				std::vector<Vertex> subMeshVertices(positionsAccessor.count);
				for (size_t j = 0; j < positionsAccessor.count; ++j) 
				{
					subMeshVertices[j].position = glm::vec3(
						positions[j * 3 + 0],
						positions[j * 3 + 1],
						positions[j * 3 + 2]
					);
					subMeshVertices[j].texCoord = glm::vec2(
						texCoords[j * 2 + 0],
						texCoords[j * 2 + 1]
					);
				}

				combinedVertices.insert(combinedVertices.end(), subMeshVertices.begin(), subMeshVertices.end());

				// Load index data
				const auto& indicesAccessor = _model.accessors[primitive.indices];
				const auto& indicesBufferView = _model.bufferViews[indicesAccessor.bufferView];
				const auto& indicesBuffer = _model.buffers[indicesBufferView.buffer];
				const uint16* indices = reinterpret_cast<const uint16*>(&(indicesBuffer.data[indicesBufferView.byteOffset + indicesAccessor.byteOffset]));
				std::vector<uint32> subMeshIndices(indicesAccessor.count);
				for (size_t j = 0; j < indicesAccessor.count; ++j) 
				{
					subMeshIndices[j] = indices[j] + subMesh.vertexOffset;
				}

				combinedIndices.insert(combinedIndices.end(), subMeshIndices.begin(), subMeshIndices.end());

				subMesh.vertices = subMeshVertices;
				subMesh.indices = subMeshIndices;
				subMesh.localTransform = nodeTransform;

				if (primitive.material > -1) 
				{
					const auto& tinyMaterial = _model.materials[primitive.material];

					if (tinyMaterial.values.find("baseColorFactor") != tinyMaterial.values.end()) 
					{
						const auto& colorFactor = tinyMaterial.values.at("baseColorFactor").ColorFactor();
						subMesh.material.SetDiffuseColor(glm::vec3(colorFactor[0], colorFactor[1], colorFactor[2]));
					}
				}

				_meshComponent->SetSubMesh(subMesh);
			}
		}

		_meshComponent->SetCombinedMeshData(combinedVertices, combinedIndices);
	}

	void ModelLoadingSystem::GetNodeTransform(const tinygltf::Node& _node, glm::mat4& _outTransform) const noexcept
	{
		if (_node.matrix.size() == 16) 
		{
			// Node has a transformation matrix
			_outTransform = glm::make_mat4(_node.matrix.data());
		}
		else 
		{
			// Node has TRS (Translation, Rotation, Scale)
			if (_node.translation.size() == 3) 
			{
				_outTransform = glm::translate(_outTransform, glm::vec3(_node.translation[0], _node.translation[1], _node.translation[2]));
			}

			if (_node.rotation.size() == 4) 
			{
				glm::quat rotation = glm::make_quat(_node.rotation.data());
				_outTransform *= glm::mat4_cast(rotation);
			}

			if (_node.scale.size() == 3) 
			{
				_outTransform = glm::scale(_outTransform, glm::vec3(_node.scale[0], _node.scale[1], _node.scale[2]));
			}
		}
	}
} // End of Banshee namespace