#pragma once

#include "Graphics/PrimitiveShape.h"
#include "Graphics/Vertex.h"
#include <vector>

namespace Banshee
{
	class ShapeFactory
	{
	public:
		static void GetShapeData(const PrimitiveShape _shape, std::vector<Vertex>& _vertices, std::vector<uint32>& _indices);
	};
} // End of namespace