#pragma once

#include "EngineConfig.h"

namespace Banshee
{
	class INIParser
	{
	public:
		static EngineConfig ParseConfigSettings(const std::string& _filePath);
	};
} // End of Banshee namespace