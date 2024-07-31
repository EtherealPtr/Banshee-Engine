#pragma once

#include "Foundation/Observer/Observer.h"
#include "Foundation/DLLConfig.h"
#include "Foundation/Components/Component.h"
#include "LightData.h"

namespace Banshee
{
	class LightComponent : public Component, public Observer
	{
	public:
		BANSHEE_ENGINE LightComponent(const glm::vec3& _color = glm::vec3(1.0f)) noexcept :
			m_LightData{ glm::vec3(0.0f), _color },
			m_NeedsUpdate{ true }
		{}

		void SetColor(const glm::vec3& _color) noexcept { m_LightData.m_Color = _color; }
		const glm::vec3& GetColor() const noexcept { return m_LightData.m_Color; }
		bool NeedsUpdate() const noexcept { return m_NeedsUpdate; }
		void ResetNeedsUpdate() noexcept { m_NeedsUpdate = false; }
		virtual void OnNotify() noexcept override;

	private:
		LightData m_LightData;
		bool m_NeedsUpdate;
	};
} // End of Banshee namespace
