#pragma once

#include "Shape.h"

namespace Banshee
{
	class Cube : public Shape
	{
	public:
		void GetShapeData(std::vector<Vertex>& _vertices, std::vector<uint32>& _indices) const override;
	};
} // End of namespace