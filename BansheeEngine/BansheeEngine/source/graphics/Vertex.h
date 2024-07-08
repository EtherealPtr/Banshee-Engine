#pragma once

#include "glm/glm.hpp"

namespace Banshee
{
	struct Vertex
	{
		Vertex(const float _pX = 0.0f, const float _pY = 0.0f, const float _pZ = 0.0f,
			   const float _u = 1.0f, const float _v = 1.0f, 
			   const float _nX = 0.0f, const float _nY = 0.0f, const float _nZ = 0.0f) noexcept :
			position(_pX, _pY, _pZ),
			texCoord(_u, _v),
			normal(_nX, _nY, _nZ)
		{}

		glm::vec3 position;
		glm::vec2 texCoord;
		glm::vec3 normal;
	};
} // End of Banshee namespace