#include "DirectionalLightComponent.h"

namespace Banshee
{
	DirectionalLightComponent::DirectionalLightComponent(const glm::vec3& _direction, const glm::vec3& _color)
	{
		m_LightData.m_PositionAndType = glm::vec4(glm::vec3(0.0f), static_cast<float>(LightType::Directional));
		m_LightData.m_Direction = glm::normalize(glm::vec4(_direction, 0.0f));
		m_LightData.m_Color = glm::vec4(_color, 1.0f);
	}
} // End of Banshee namespace