#include "DirectionalLightComponent.h"

namespace Banshee
{
	void DirectionalLightComponent::SetDirection(const glm::vec3& _direction)
	{
		m_Direction = _direction;
		m_LightData.m_Position = glm::normalize(m_Direction);
	}
} // End of Banshee namespace