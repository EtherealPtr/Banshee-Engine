#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/Components/Component.h"
#include "LightData.h"

namespace Banshee
{
	class LightComponent : public Component
	{
	public:
		BANSHEE_ENGINE explicit LightComponent(const LightData _lightData) noexcept :
			m_LightData{ _lightData }
		{}

		void SetColor(const glm::vec3& _color) noexcept { m_LightData.m_Color = glm::vec4(_color, 0.0f); }
		glm::vec3 GetColor() const noexcept { return m_LightData.m_Color; }

	protected:
		LightData m_LightData;
	};
} // End of Banshee namespace