#pragma once

#include "EngineConfig.h"

namespace Banshee
{
	class INIParser
	{
	public:
		EngineConfig& ParseConfigSettings(const std::string& _filePath);

	private:
		EngineConfig m_Config{};
	};
} // End of Banshee namespace