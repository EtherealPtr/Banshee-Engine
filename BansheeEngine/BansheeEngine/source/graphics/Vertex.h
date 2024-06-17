#pragma once

#include "glm/glm.hpp"

namespace Banshee
{
	struct Vertex
	{
		Vertex(const float _pX = 0.0f, const float _pY = 0.0f, const float _pZ = 0.0f,
			   const float _u = 1.0f, const float _v = 1.0f) noexcept :
			position(_pX, _pY, _pZ),
			texCoord(_u, _v)
		{}

		glm::vec3 position;
		glm::vec2 texCoord;
	};
} // End of Banshee namespace