#pragma once

#include "Foundation/Platform.h"
#include <glm/glm.hpp>

namespace Banshee
{
	enum class LightType : uint8
	{
		Directional,
		Point
	};

	struct LightData
	{
		LightData(const LightType _type = LightType::Point, const glm::vec3& _color = glm::vec3(1.0f), const glm::vec3& _position = glm::vec3(0.0f), const glm::vec3& _direction = glm::vec3(0.0f), const float _constant = 1.0f, const float _linear = 0.09f, const float _quadratic = 0.032f) noexcept :
			m_TypeAndPosition{ glm::vec4(static_cast<float>(_type), _position) },
			m_Direction{ glm::vec4(_direction, 0.0f) },
			m_Color{ glm::vec4(_color, 0.0f) },
			m_Attenuation{ glm::vec4(_constant, _linear, _quadratic, 0.0f) }
		{}

		alignas(16) glm::vec4 m_TypeAndPosition;	// First element is type, other 3 are positions
		alignas(16) glm::vec4 m_Direction;			// Last element is padding
		alignas(16) glm::vec4 m_Color;				// Last element is padding
		alignas(16) glm::vec4 m_Attenuation;		// Last element is padding
	};
} // End of Banshee namespace