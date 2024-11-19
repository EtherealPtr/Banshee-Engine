#pragma once

#include "LightComponent.h"

namespace Banshee
{
	class SpotLightComponent : public LightComponent
	{
	public:
		BANSHEE_ENGINE SpotLightComponent(const glm::vec3& _direction = glm::vec3(0.0f, -1.0f, 0.0f), const glm::vec4& _color = glm::vec4(1.0f), const float _radius = 10.0f, const float _innerCutoffAngle = 12.5f, const float _outerCutoffAngle = 17.5f);
		
		virtual void UpdatePosition() override;
		void SetPosition(const glm::vec3& _pos);

	private:
		void UpdateAttenuation(const float _radius, const float _innerCutoffAngle, const float _outerCutoffAngle);
	};
} // End of Banshee namespace