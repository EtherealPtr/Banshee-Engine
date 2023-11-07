#pragma once

#include <glm/glm.hpp>

namespace Banshee
{
	struct ViewProjMatrix
	{
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 proj = glm::mat4(1.0f);
	};

	struct ModelMatrix
	{
		glm::mat4 model = glm::mat4(1.0f);
	};
} // End of Banshee namespace