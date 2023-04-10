#pragma once

#include "Subsystem.h"
#include "EngineConfig.h"

namespace Banshee
{
	class INIParser : public Subsystem
	{
	public:
		INIParser() {};

		EngineConfig ParseConfigSettings(const char* _filePath);
	};
} // End of Banshee namespace