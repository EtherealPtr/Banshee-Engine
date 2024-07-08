#pragma once

#include <glm/glm.hpp>

namespace Banshee
{
	struct LightData
	{
		LightData(const glm::vec3& _position = glm::vec3(0.0f), const glm::vec3& _color = glm::vec3(1.0f)) noexcept :
			position(_position),
			color(_color)
		{}

		alignas(16) glm::vec3 position;
		alignas(16) glm::vec3 color;
	};
} // End of Banshee namespace