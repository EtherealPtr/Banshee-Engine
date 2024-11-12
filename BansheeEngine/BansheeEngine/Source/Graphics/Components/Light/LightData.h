#pragma once

#include "Foundation/Platform.h"
#include <glm/glm.hpp>

namespace Banshee
{
	enum class LightType : uint8
	{
		Directional,
		Point,
		Spotlight
	};

	struct LightData
	{
		LightData() noexcept :
			m_PositionAndType{ glm::vec4(0.0f) },
			m_Direction{ glm::vec4(0.0f) },
			m_Color{ glm::vec4(0.0f) },
			m_Attenuation{ glm::vec4(0.0f) }
		{}

		glm::vec4 m_PositionAndType;	// Last element is type
		glm::vec4 m_Direction;			// Last element is padding
		glm::vec4 m_Color;				// Last element is color intensity
		glm::vec4 m_Attenuation;		// Last element is the outer cutoff angle
	};

	struct LightBuffer
	{
		alignas(16) uint32 m_TotalLights{ 0 };
		alignas(16) LightData m_Lights[50]{};
	};
} // End of Banshee namespace