#include "Logger.h"

namespace Banshee
{
	const Logger g_Logger;

	Logger::Logger()
		: m_LogFile{}
	{
		if (!m_LogFile.is_open())
		{
			m_LogFile.open("C:/Projects/Banshee-Engine/BansheeEngine/BansheeEngine/Banshee_Generated/logs.txt", std::ios::out | std::ios::app);
			if (!m_LogFile)
			{
				throw std::runtime_error("ERROR: Failed to open log file.");
			}
		}

		m_LogEvent = [this](const char* logData) { WriteToLogFile(logData); };
	}

	Logger::~Logger()
	{
		if (m_LogFile.is_open())
		{
			m_LogFile.close();
		}
	}

	void Logger::WriteToLogFile(const char* _logData)
	{
		if (m_LogFile.is_open())
		{
			m_LogFile << _logData << '\n';
		}
	}
} // End of Banshee namespace