#pragma once

#include <glm/glm.hpp>

namespace Banshee
{
	struct LightData
	{
		LightData(const glm::vec3& _position = glm::vec3(0.0f), const glm::vec3& _color = glm::vec3(1.0f)) noexcept :
			m_Position{ _position },
			m_Color{ _color }
		{}

		alignas(16) glm::vec3 m_Position;
		alignas(16) glm::vec3 m_Color;
	};
} // End of Banshee namespace