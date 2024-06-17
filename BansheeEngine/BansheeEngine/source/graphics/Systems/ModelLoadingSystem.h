#pragma once

#include "glm/glm.hpp"

namespace tinygltf
{
	class Model;
	class Node;
}

namespace Banshee
{
	class MeshComponent;

	class ModelLoadingSystem
	{
	public:
		ModelLoadingSystem(const char* _modelPath, MeshComponent* _meshComponent);
		~ModelLoadingSystem() = default;

		ModelLoadingSystem(const ModelLoadingSystem&) = delete;
		ModelLoadingSystem(ModelLoadingSystem&&) = delete;
		void operator=(const ModelLoadingSystem&) = delete;
		void operator=(ModelLoadingSystem&&) = delete;

	private:
		void LoadModel(const tinygltf::Model& _model, MeshComponent* _meshComponent);
		void GetNodeTransform(const tinygltf::Node& _node, glm::mat4& _outTransform) const noexcept;)
	};
} // End of Banshee namespace