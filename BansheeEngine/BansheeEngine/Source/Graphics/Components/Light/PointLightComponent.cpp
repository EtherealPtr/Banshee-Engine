#include "PointLightComponent.h"

namespace Banshee
{
	PointLightComponent::PointLightComponent(const glm::vec3& _color, const float _constant, const float _linear, const float _quadratic) noexcept : 
		LightComponent(LightData(LightType::Point, _color, glm::vec3(0.0f), glm::vec3(0.0f), _constant, _linear, _quadratic)),
		m_Constant(_constant),
		m_Linear(_linear),
		m_Quadratic(_quadratic)
	{}

	void PointLightComponent::SetAttenuation(const float _constant, const float _linear, const float _quadratic) noexcept
	{
		m_Constant = _constant;
		m_Linear = _linear;
		m_Quadratic = _quadratic;
	}
} // End of Banshee namespace