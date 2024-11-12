#pragma once

#include "LightComponent.h"

namespace Banshee
{
	class DirectionalLightComponent : public LightComponent
	{
	public:
		BANSHEE_ENGINE DirectionalLightComponent(const glm::vec3& _direction = glm::vec3(0.0f, -1.0f, 0.0f), const glm::vec3& _color = glm::vec3(1.0f));
	};
} // End of Banshee namespace