#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/Components/Component.h"
#include "LightData.h"

namespace Banshee
{
	class LightComponent : public Component
	{
	public:
		BANSHEE_ENGINE LightComponent(const glm::vec3& _color = glm::vec3(1.0f)) noexcept :
			m_LightData{ glm::vec3(0.0f), _color }
		{}

		void SetColor(const glm::vec3& _color) noexcept { m_LightData.m_Color = _color; }
		const glm::vec3& GetColor() const noexcept { return m_LightData.m_Color; }

	protected:
		LightData m_LightData;
	};
} // End of Banshee namespace