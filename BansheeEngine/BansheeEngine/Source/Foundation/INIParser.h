#pragma once

#include "EngineConfig.h"

namespace Banshee
{
	class INIParser
	{
	public:
		INIParser() {};

		EngineConfig ParseConfigSettings(const char* _filePath);
	};
} // End of Banshee namespace