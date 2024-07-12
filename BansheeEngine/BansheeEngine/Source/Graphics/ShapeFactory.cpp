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
				Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
				Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
				Vertex(0.0f, 0.5f, 0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f)
			};

			_indices = { 0, 1, 2 };
			break;

		case PrimitiveShape::Square:
			_vertices =
			{
				Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),
				Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
				Vertex(0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
				Vertex(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f)
			};

			_indices = { 0, 1, 2, 2, 3, 0 };
			break;

		case PrimitiveShape::Pyramid:
			_vertices =
			{
				Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f, -0.5f, -0.5f, 0.0f),
				Vertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f, 0.5f, -0.5f, 0.0f),
				Vertex(0.5f, -0.5f, -1.0f, 1.0f, 0.0f, 0.5f, -0.5f, -1.0f),
				Vertex(-0.5f, -0.5f, -1.0f, 0.0f, 0.0f, -0.5f, -0.5f, -1.0f),
				Vertex(0.0f, 0.5f, -0.5f, 0.5f, 0.5f, 0.0f, 0.5f, -0.5f)
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

		case PrimitiveShape::Cube:
			_vertices =
			{
				// Front face
				Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f),
				Vertex(0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f),
				Vertex(0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f),
				Vertex(-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f),

				// Back face
				Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f),
				Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f),
				Vertex(0.5f,  0.5f, -0.5f, 1.0f, 1.0f, 0.0f, 0.0f, -1.0f),
				Vertex(-0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f),

				// Left face
				Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f),
				Vertex(-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f),
				Vertex(-0.5f,  0.5f,  0.5f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f),
				Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f),

				// Right face
				Vertex(0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f),
				Vertex(0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f),
				Vertex(0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f),
				Vertex(0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f),

				// Top face
				Vertex(-0.5f,  0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f),
				Vertex(0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f),
				Vertex(0.5f,  0.5f,  0.5f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f),
				Vertex(-0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f),

				// Bottom face
				Vertex(-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f),
				Vertex(0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f),
				Vertex(0.5f, -0.5f,  0.5f, 1.0f, 1.0f, 0.0f, -1.0f, 0.0f),
				Vertex(-0.5f, -0.5f,  0.5f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f),
			};

			_indices =
			{
				0,  1,  2,  2,  3,  0,  // Front face
				4,  5,  6,  6,  7,  4,  // Back face
				8,  9, 10, 10, 11,  8,  // Left face
				12, 13, 14, 14, 15, 12, // Right face
				16, 17, 18, 18, 19, 16, // Top face
				20, 21, 22, 22, 23, 20  // Bottom face
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