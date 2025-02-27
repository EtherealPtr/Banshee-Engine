#include "ShapeFactory.h"
#include "Triangle.h"
#include "Square.h"
#include "Pyramid.h"
#include "Cube.h"

namespace Banshee
{
	void ShapeFactory::GetShapeData(const PrimitiveShape _shape, std::vector<Vertex>& _vertices, std::vector<uint32>& _indices)
	{
		switch (_shape)
		{
			case PrimitiveShape::TriangleShape:
			{
				Triangle triangle{};
				triangle.GetShapeData(_vertices, _indices);
				break;
			}
			case PrimitiveShape::SquareShape:
			{
				Square square{};
				square.GetShapeData(_vertices, _indices);
				break;
			}
			case PrimitiveShape::PyramidShape:
			{
				Pyramid pyramid{};
				pyramid.GetShapeData(_vertices, _indices);
				break;
			}
			case PrimitiveShape::CubeShape:
			{
				Cube cube{};
				cube.GetShapeData(_vertices, _indices);
				break;
			}
		}
	}
} // End of namespace