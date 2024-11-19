#include "SpotLightComponent.h"
#include "Foundation/Entity/Entity.h"
#include "Graphics/Components/Transform/TransformComponent.h"

namespace Banshee
{
	SpotLightComponent::SpotLightComponent(const glm::vec3& _direction, const glm::vec4& _color, const float _radius, const float _innerCutoffAngle, const float _outerCutoffAngle)
	{
		m_LightData.m_Direction = glm::normalize(glm::vec4(_direction, 0.0f));
		m_LightData.m_Color = _color;

		UpdateAttenuation(_radius, _innerCutoffAngle, _outerCutoffAngle);
	}

	void SpotLightComponent::UpdatePosition()
	{
		const auto transform{ GetOwner()->GetComponent<TransformComponent>() };
		SetPosition(transform->GetPosition());
	}

	void SpotLightComponent::SetPosition(const glm::vec3& _pos)
	{
		m_LightData.m_PositionAndType = glm::vec4(_pos, static_cast<float>(LightType::Spotlight));
	}

	void SpotLightComponent::UpdateAttenuation(const float _radius, const float _innerCutoffAngle, const float _outerCutoffAngle)
	{
		constexpr float constant{ 1.0f };
		const float linear{ 4.5f / _radius };
		const float quadratic{ 75.0f / (_radius * _radius) };

		m_LightData.m_Attenuation = glm::vec4(constant, linear, glm::cos(glm::radians(_innerCutoffAngle)), glm::cos(glm::radians(_outerCutoffAngle)));
	}
} // End of Banshee namespace