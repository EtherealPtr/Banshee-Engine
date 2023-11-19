#include "INIParser.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/ResourceManager/File/FileManager.h"
#include "Foundation/Logging/Logger.h"
#include <sstream>

namespace Banshee
{
	EngineConfig INIParser::ParseConfigSettings(const char* _filePath)
	{
		EngineConfig config{};

		std::ifstream file = ResourceManager::Instance().GetFileManager()->ReadFile(_filePath);

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
					config.windowTitle = value;
				}
				else if (key == "WindowWidth")
				{
					config.windowWidth = static_cast<uint16>(std::stoi(value));
				}
				else if (key == "WindowHeight")
				{
					config.windowHeight = static_cast<uint16>(std::stoi(value));
				}
			}
		}

		BE_LOG(LogCategory::Info, "[CONFIG]: Loaded config.ini");
		return config;
	}
} // End of Banshee namespace