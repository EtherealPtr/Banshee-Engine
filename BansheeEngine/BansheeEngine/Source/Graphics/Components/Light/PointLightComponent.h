#pragma once

#include "LightComponent.h"

namespace Banshee
{
	class PointLightComponent : public LightComponent
	{
	public:
		BANSHEE_ENGINE PointLightComponent(const glm::vec4& _color = glm::vec4(1.0f), const float _radius = 10.0f);
		
		virtual void UpdatePosition() override;

	private:
		void SetPosition(const glm::vec3& _pos);
		void UpdateAttenuation(const float _radius);
	};
} // End of Banshee namespace