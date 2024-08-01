#pragma once

#include "EngineConfig.h"

namespace Banshee
{
	class INIParser
	{
	public:
		const EngineConfig& ParseConfigSettings(std::string_view _filePath);

	private:
		EngineConfig m_Config{};
	};
} // End of Banshee namespace