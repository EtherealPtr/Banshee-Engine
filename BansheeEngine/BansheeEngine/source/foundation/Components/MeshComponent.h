#pragma once

#include "Component.h"
#include "Foundation/Platform.h"
#include "Foundation/DLLConfig.h"

namespace Banshee
{
	class MeshComponent : public Component
	{
	public:
		BANSHEE_ENGINE MeshComponent(uint8 _shape = 0) : 
			m_Shape(_shape),
			m_Color(3, 1.0f)
		{}

		void SetColor(const float _r, const float _g, const float _b)
		{
			m_Color[0] = _r;
			m_Color[1] = _g;
			m_Color[2] = _b;
		}

		const std::vector<float>& GetColor() const { return m_Color; }
		uint8 GetShape() const { return m_Shape; }

	private:
		uint8 m_Shape;
		std::vector<float> m_Color;
	};
} // End of Banshee namespace