#pragma once

#include "Vertex.h"
#include "PrimitiveShape.h"
#include <vector>

namespace Banshee
{
	class MeshComponent
	{
	public:
		MeshComponent(const PrimitiveShape _shape = PrimitiveShape::empty);
		std::vector<Vertex> GetVertices() const { return m_Vertices; }
		uint64 GetVertexSizeInBytes() const { return sizeof(Vertex) * m_Vertices.size(); }

	private:
		std::vector<Vertex> m_Vertices;
	};
} // End of Banshee namespace