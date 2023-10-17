#pragma once

#include "Component.h"
#include "Foundation/PrimitiveShape.h"
#include "Foundation/DLLConfig.h"

namespace Banshee
{
	class MeshRendererComponent : public Component
	{
	public:
		BANSHEE_ENGINE MeshRendererComponent(PrimitiveShape _shape = PrimitiveShape::Triangle) : 
			m_Shape(_shape)
		{}

		PrimitiveShape GetShape() const { return m_Shape; }

	private:
		PrimitiveShape m_Shape;
	};
} // End of Banshee namespace