#include "ShapeFactory.h"

namespace Banshee
{
	void ShapeFactory::GetShapeData(const PrimitiveShape _shape, std::vector<Vertex>& _vertices, std::vector<uint32>& _indices)
	{
		switch (_shape)
		{
		case PrimitiveShape::Triangle:
			_vertices =
			{
				Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f),
				Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f),
				Vertex(0.0f, 0.5f, 0.0f, 0.5f, 0.0f)
			};

			_indices = { 0, 1, 2 };
			break;

		case PrimitiveShape::Square:
			_vertices =
			{
				Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f),
				Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f),
				Vertex(0.5f, 0.5f, 0.0f, 1.0f, 0.0f),
				Vertex(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f)
			};

			_indices = { 0, 1, 2, 2, 3, 0 };
			break;

		case PrimitiveShape::Pyramid:
			_vertices =
			{
				Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f),
				Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f),
				Vertex(0.5f, -0.5f, -1.0f, 1.0f, 0.0f),
				Vertex(-0.5f, -0.5f, -1.0f, 0.0f, 0.0f),
				Vertex(0.0f, 0.5f, -0.5f, 0.5f, 0.5f)
			};

			_indices =
			{
				0, 3, 4,
				3, 2, 4,
				2, 1, 4,
				1, 0, 4,
				1, 2, 3,
				1, 3, 0
			};
			break;
		}
	}

	uint32 ShapeFactory::GetIndicesCount(const PrimitiveShape _shape) noexcept
	{
		switch (_shape)
		{
		case PrimitiveShape::Triangle: return 3;
		case PrimitiveShape::Square: return 6;
		case PrimitiveShape::Pyramid: return 18;
		}

		return 0;
	}
} // End of Banshee namespace