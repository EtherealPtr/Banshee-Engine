#pragma once

#include "glm/glm.hpp"

namespace Banshee
{
	struct Vertex
	{
		Vertex(const float _pX = 0.0f, const float _pY = 0.0f, const float _pZ = 0.0f,
			const float _r = 1.0f, const float _g = 1.0f, const float _b = 1.0f,
			const float _u = 1.0f, const float _v = 1.0f) :
			position(_pX, _pY, _pZ),
			color(_r, _g, _b),
			texCoord(_u, _v)
		{}

		glm::vec3 position;
		glm::vec3 color;
		glm::vec2 texCoord;
	};
}
