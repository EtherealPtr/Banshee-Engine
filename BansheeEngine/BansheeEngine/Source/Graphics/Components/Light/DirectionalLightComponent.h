#pragma once

#include "LightComponent.h"

namespace Banshee
{
	class DirectionalLightComponent : public LightComponent
	{
	public:
		BANSHEE_ENGINE DirectionalLightComponent(const glm::vec3& _direction = glm::vec3(0.0f, -1.0f, 0.0f), const glm::vec3& _color = glm::vec3(1.0f)) noexcept;

		void SetDirection(const glm::vec3& _direction);
		const glm::vec3& GetDirection() const noexcept { return m_Direction; }

	private:
		glm::vec3 m_Direction;
	};
} // End of Banshee namespace