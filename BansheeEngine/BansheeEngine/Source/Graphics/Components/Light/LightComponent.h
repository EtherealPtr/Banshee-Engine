#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/Components/Component.h"
#include "LightData.h"

namespace Banshee
{
	class LightComponent : public Component
	{
	public:
		const LightData& GetLightData() const noexcept { return m_LightData; }
		virtual void UpdatePosition() noexcept {};

	protected:
		LightData m_LightData;
	};
} // End of Banshee namespace