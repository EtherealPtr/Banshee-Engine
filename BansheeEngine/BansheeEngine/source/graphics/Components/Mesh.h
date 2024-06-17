#pragma once

#include "Foundation/Platform.h"
#include "Graphics/Vertex.h"
#include <vector>
#include "Material.h"

namespace Banshee
{
	struct Mesh
	{
		Mesh() noexcept :
			vertexOffset{ 0 },
			indexOffset{ 0 },
			materialIndex{ 0 },
			vertices{},
			indices{},
			material{},
			modelMatrix{}
		{}

		uint32 vertexOffset;  // Offset into the vertex buffer
		uint32 indexOffset;   // Offset into the index buffer
		uint32 materialIndex;
		std::vector<Vertex> vertices{};
		std::vector<uint32> indices{};
		Material material;
		glm::mat4 modelMatrix;
	};
} // End of Banshee namespace