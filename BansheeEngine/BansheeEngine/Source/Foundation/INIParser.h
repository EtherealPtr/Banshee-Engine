#pragma once

#include "EngineConfig.h"

namespace Banshee
{
	class INIParser
	{
	public:
		static EngineConfig ParseConfigSettings(std::string_view _filePath);
	};
} // End of namespace