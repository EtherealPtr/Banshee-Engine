#pragma once

#include "Foundation/Platform.h"
#include "Graphics/Vertex.h"
#include <string>
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
	class MeshSystem;
	class MeshComponent;
	class Mesh;

	class ModelLoadingSystem
	{
	public:
		ModelLoadingSystem(MeshComponent& _meshComponent, MeshSystem& _meshSystem, std::vector<Vertex>& _vertices, std::vector<uint32>& _indices);
		~ModelLoadingSystem() = default;

		ModelLoadingSystem(const ModelLoadingSystem&) = delete;
		ModelLoadingSystem(ModelLoadingSystem&&) = delete;
		ModelLoadingSystem& operator=(const ModelLoadingSystem&) = delete;
		ModelLoadingSystem& operator=(ModelLoadingSystem&&) = delete;

	private:
		void LoadModel(const tinygltf::Model& _model, MeshComponent& _meshComponent, MeshSystem& _meshSystem, std::vector<Vertex>& _vertices, std::vector<uint32>& _indices);
		void GetNodeTransform(const tinygltf::Node& _node, glm::mat4& _outTransform) const noexcept;
		void LoadMaterial(const tinygltf::Model& _model, const tinygltf::Primitive& _primitive, Mesh* const _subMesh);

	private:
		std::vector<uint16> m_TextureIds;
		std::unordered_map<uint16, uint16> m_TextureIdMap;
	};
} // End of Banshee namespace