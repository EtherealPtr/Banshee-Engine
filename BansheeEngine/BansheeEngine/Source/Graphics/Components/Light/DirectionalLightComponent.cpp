#include "DirectionalLightComponent.h"

namespace Banshee
{
	DirectionalLightComponent::DirectionalLightComponent(const glm::vec3& _direction, const glm::vec3& _color) noexcept :
		LightComponent(LightData(LightType::Directional, _color, glm::vec3(0.0f), _direction)),
		m_Direction(_direction)
	{}

	void DirectionalLightComponent::SetDirection(const glm::vec3& _direction)
	{
		m_Direction = _direction;
		m_LightData.m_Direction = glm::normalize(glm::vec4(m_Direction, 0.0f));
	}
} // End of Banshee namespace