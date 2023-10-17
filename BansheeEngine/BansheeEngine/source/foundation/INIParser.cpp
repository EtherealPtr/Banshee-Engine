#include "INIParser.h"
#include "Foundation/File/FileManager.h"
#include "Foundation/Logging/Logger.h"
#include <sstream>

namespace Banshee
{
	EngineConfig INIParser::ParseConfigSettings(const char* _filePath)
	{
		EngineConfig config{};

		std::ifstream file = FileManager::Instance().ReadFile(_filePath);

		std::string line = "";
		while (std::getline(file, line))
		{
			// Skip comments or empty lines
			if (line.empty() || line[0] == '#' || line[0] == ';') continue;

			// Split the line into key-value pairs
			std::istringstream iss(line);
			std::string key = "", value = "";

			if (std::getline(std::getline(iss, key, '='), value))
			{
				if (key == "WindowTitle")
				{
					config.m_WindowTitle = value;
				}
				else if (key == "WindowWidth")
				{
					config.m_WindowWidth = static_cast<uint16>(std::stoi(value));
				}
				else if (key == "WindowHeight")
				{
					config.m_WindowHeight = static_cast<uint16>(std::stoi(value));
				}
			}
		}

		return config;
	}
} // End of Banshee namespace