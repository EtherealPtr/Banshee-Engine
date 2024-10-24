#pragma once

#include "LightComponent.h"

namespace Banshee
{
	class PointLightComponent : public LightComponent
	{
	public:
		BANSHEE_ENGINE PointLightComponent(const glm::vec3& _color = glm::vec3(1.0f), const float _constant = 1.0f, const float _linear = 0.09f, const float _quadratic = 0.032f) noexcept;

		void SetAttenuation(const float _constant, const float _linear, const float _quadratic) noexcept;
		float GetConstant() const noexcept { return m_Constant; }
		float GetLinear() const noexcept { return m_Linear; }
		float GetQuadratic() const noexcept { return m_Quadratic; }

	private:
		float m_Constant;
		float m_Linear;
		float m_Quadratic;
	};
} // End of Banshee namespace