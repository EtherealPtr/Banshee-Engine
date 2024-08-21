#include "INIParser.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/Logging/Logger.h"

namespace Banshee
{
	const EngineConfig& INIParser::ParseConfigSettings(std::string_view _filePath)
	{
		std::ifstream file{ g_ResourceManager.ReadFile(_filePath.data()) };
		if (!file.is_open())
		{
			return m_Config;
		}

		std::string line{ "" };

		while (std::getline(file, line))
		{
			// Skip comments or empty lines
			if (line.empty() || line.front() == '#' || line.front() == ';')
			{
				continue;
			}

			const size_t delimiterPos{ line.find('=') };
			if (delimiterPos == std::string_view::npos)
			{
				continue;
			}

			const std::string_view key{ std::string_view(line).substr(0, delimiterPos) };
			const std::string_view value{ std::string_view(line).substr(delimiterPos + 1) };

			if (key == "WindowTitle")
			{
				m_Config.m_WindowTitle = value;
			}
			else if (key == "WindowWidth")
			{
				m_Config.m_WindowWidth = std::stoul(std::string(value));
			}
			else if (key == "WindowHeight")
			{
				m_Config.m_WindowHeight = std::stoul(std::string(value));
			}
		}

		BE_LOG(LogCategory::Info, "[CONFIG]: Loaded config.ini");
		return m_Config;
	}
} // End of Banshee namespace