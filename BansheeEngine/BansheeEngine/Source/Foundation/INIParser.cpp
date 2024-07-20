#include "INIParser.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/ResourceManager/File/FileManager.h"
#include "Foundation/Logging/Logger.h"
#include <sstream>

namespace Banshee
{
	EngineConfig INIParser::ParseConfigSettings(const std::string& _filePath)
	{
		EngineConfig config{};

		std::ifstream file = ResourceManager::Instance().GetFileManager()->ReadFile(_filePath.c_str());
		if (!file.is_open()) 
		{
			return config;
		}

		std::string line = "";

		while (std::getline(file, line)) 
		{
			// Skip comments or empty lines
			if (line.empty() || line.front() == '#' || line.front() == ';') 
			{
				continue;
			}

			size_t delimiterPos = line.find('=');
			if (delimiterPos == std::string::npos) 
			{
				continue;
			}

			const std::string key = line.substr(0, delimiterPos);
			const std::string value = line.substr(delimiterPos + 1);

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

		BE_LOG(LogCategory::Info, "[CONFIG]: Loaded config.ini");
		return config;
	}
} // End of Banshee namespace