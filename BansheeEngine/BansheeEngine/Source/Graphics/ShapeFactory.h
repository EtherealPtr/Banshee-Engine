#pragma once

#include "Foundation/Platform.h"
#include "PrimitiveShape.h"
#include "Vertex.h"
#include <vector>

namespace Banshee
{
	class ShapeFactory
	{
	public:
		static void GetShapeData(const PrimitiveShape _shape, std::vector<Vertex>& _vertices, std::vector<uint32>& _indices);
		static uint32 GetIndicesCount(const PrimitiveShape _shape) noexcept;
	};
} // End of Banshee namespace