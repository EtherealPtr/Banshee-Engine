#include "Logger.h"
#include "FileManager.h"
#include <stdarg.h>

#define STRINGIFY(text) #text

constexpr auto g_Reset	= "\033[0m";
constexpr auto g_Red	= "\033[31m";
constexpr auto g_Green	= "\033[32m";
constexpr auto g_Yellow	= "\033[33m";

namespace Banshee
{
	static Logger s_Logger;

	Logger& Logger::Instance()
	{
		static Logger s_Logger;
		return s_Logger;
	}

	void Logger::PrintLog(const LogCategory _category, const char* _format, ...)
	{
		va_list argList = nullptr;
		va_start(argList, _format);

		const std::string categoryText = GetLogCategoryName(_category);

		const uint16 size = 1024;
		char userLog[size]{};
		vsnprintf_s(userLog, size, _format, argList);
		va_end(argList);

		std::string outputLog = categoryText + " " + userLog;

		switch (_category)
		{
		case LogCategory::trace:	printf("%s%s\n", g_Reset, outputLog.c_str()); break;
		case LogCategory::info:		printf("%s%s\n", g_Green, outputLog.c_str()); break;
		case LogCategory::warning:	printf("%s%s\n", g_Yellow, outputLog.c_str()); break;
		case LogCategory::error:	printf("%s%s\n", g_Red, outputLog.c_str()); break;
		default:					printf("%s\n", outputLog.c_str());
		}

		FileManager::Instance().WriteToLogFile(outputLog.c_str());
	}

	std::string Logger::GetLogCategoryName(const LogCategory _category)
	{
		switch (_category)
		{
		case LogCategory::trace:	return STRINGIFY([TRACE]);
		case LogCategory::info:		return STRINGIFY([INFO]);
		case LogCategory::warning:	return STRINGIFY([WARNING]);
		case LogCategory::error:	return STRINGIFY([ERROR]);
		default:					return "";
		}
	}
} // End of Banshee namespace