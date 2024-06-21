#pragma once

#include "Foundation/Platform.h"
#include "Graphics/Vertex.h"
#include <vector>
#include <unordered_map>

namespace tinygltf
{
	class Model;
	class Node;
	struct Primitive;
}

namespace Banshee
{
	class MeshComponent;
	struct Mesh;

	class ModelLoadingSystem
	{
	public:
		ModelLoadingSystem(const char* _modelPath, MeshComponent* _meshComponent, std::vector<Vertex>& _vertices, std::vector<uint32>& _indices);
		~ModelLoadingSystem() = default;

		ModelLoadingSystem(const ModelLoadingSystem&) = delete;
		ModelLoadingSystem(ModelLoadingSystem&&) = delete;
		void operator=(const ModelLoadingSystem&) = delete;
		void operator=(ModelLoadingSystem&&) = delete;

	private:
		void LoadModel(const tinygltf::Model& _model, MeshComponent* _meshComponent, std::vector<Vertex>& _vertices, std::vector<uint32>& _indices);
		void GetNodeTransform(const tinygltf::Node& _node, glm::mat4& _outTransform) const noexcept;
		void LoadMaterial(const tinygltf::Model& _model, const tinygltf::Primitive& _primitive, Mesh* _subMesh);

	private:
		std::vector<uint16> m_TextureIds;
		std::unordered_map<int, uint16> m_TextureIdMap;
	};
} // End of Banshee namespace