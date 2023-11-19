#include "ShapeFactory.h"

namespace Banshee
{
	void ShapeFactory::GetVertices(const PrimitiveShape _shape, std::vector<Vertex>& _vertices, std::vector<uint16>& _indices)
	{
		switch (_shape)
		{
		case PrimitiveShape::Triangle:
			_vertices =
			{
				Vertex(-0.5f, 0.5f, 0.0f, 0.0f, 1.0f),
				Vertex(0.0f, -0.5f, 0.0f, 0.5f, 0.0f),
				Vertex(0.5f,  0.5f, 0.0f, 1.0f, 1.0f)
			};

			_indices = { 0, 1, 2 };
			break;

		case PrimitiveShape::Square:
			_vertices =
			{
				Vertex(-0.5f,  0.5f, 0.0f, 0.0f, 1.0f),
				Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f),
				Vertex( 0.5f, -0.5f, 0.0f, 1.0f, 0.0f),
				Vertex( 0.5f,  0.5f, 0.0f, 1.0f, 1.0f)
			};

			_indices = { 0, 1, 2, 2, 3, 0 };
			break;
		}
	}

	uint16 ShapeFactory::GetIndicesCount(const PrimitiveShape _shape) noexcept
	{
		switch (_shape)
		{
		case PrimitiveShape::Triangle: return 3;
		case PrimitiveShape::Square: return 6;
		}

		return 0;
	}
} // End of Banshee namespace