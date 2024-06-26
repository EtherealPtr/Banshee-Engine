#pragma once

#include <glm/glm.hpp>

namespace Banshee
{
	struct LightData
	{
		LightData(const glm::vec3& _location, const glm::vec3& _color = glm::vec3(1.0f)) noexcept :
			location(_location),
			color(_color)
		{}

		glm::vec3 location;
		glm::vec3 color;
	};
} // End of Banshee namespace