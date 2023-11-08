#pragma once

#include "Foundation/PrimitiveShape.h"
#include "Foundation/Platform.h"
#include "Vertex.h"
#include <vector>

namespace Banshee
{
	class ShapeFactory
	{
	public:
		static void GetVertices(const PrimitiveShape _shape, std::vector<Vertex>& _vertices, std::vector<uint16>& _indices);
		static uint16 GetIndicesCount(const PrimitiveShape _shape);
	};
} // End of Banshee namespace