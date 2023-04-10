#include "MeshComponent.h"

namespace Banshee
{
	MeshComponent::MeshComponent(const PrimitiveShape _shape)
	{
		switch (_shape)
		{
		case PrimitiveShape::triangle:

			m_Vertices.resize(3);
			m_Vertices[0] = Vertex(-0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f);
			m_Vertices[1] = Vertex( 0.0f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f);
			m_Vertices[2] = Vertex( 0.5f,  0.5f, 0.0f,  0.0f, 1.0f, 0.0f);
			break;
		}
	}
} // End of Banshee namespace