#pragma once

#include "Platform.h"
#include <string>

namespace Banshee
{
	struct EngineConfig
	{
		EngineConfig() :
			windowWidth(400),
			windowHeight(300),
			windowTitle("Untitled")
		{}

		uint16 windowWidth;
		uint16 windowHeight;
		std::string windowTitle;
	};
} // End of Banshee namespace