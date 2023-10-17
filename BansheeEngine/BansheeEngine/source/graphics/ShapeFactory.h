#pragma once

#include "Foundation/PrimitiveShape.h"
#include "Vertex.h"
#include <vector>
#include <cstdint>

namespace Banshee
{
	class ShapeFactory
	{
	public:
		static void GetVertices(const PrimitiveShape _shape, std::vector<Vertex>& _vertices, std::vector<uint16_t>& _indices);
		static uint16_t GetIndicesCount(const PrimitiveShape _shape);
	};
}
