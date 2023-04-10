#pragma once

#include "Platform.h"
#include <string>

namespace Banshee
{
	struct EngineConfig
	{
		EngineConfig() : 
			m_WindowWidth(400),
			m_WindowHeight(300),
			m_WindowTitle("Untitled")
		{}

		uint16 m_WindowWidth;
		uint16 m_WindowHeight;
		std::string m_WindowTitle;
	};
}; // End of Banshee namespace