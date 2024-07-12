#include "Logger.h"
#include "Foundation/ResourceManager/ResourceManager.h"
#include "Foundation/ResourceManager/File/FileManager.h"
#include <stdarg.h>

namespace Banshee
{
	#define STRINGIFY(text) #text
	
	constexpr auto g_Reset	= "\033[0m";
	constexpr auto g_Red	= "\033[31m";
	constexpr auto g_Green	= "\033[32m";
	constexpr auto g_Yellow	= "\033[33m";

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

		const std::string outputLog = categoryText + userLog;

		switch (_category)
		{
		case LogCategory::Trace:	printf("%s%s\n", g_Reset, outputLog.c_str()); break;
		case LogCategory::Info:		printf("%s%s\n", g_Green, outputLog.c_str()); break;
		case LogCategory::Warning:	printf("%s%s\n", g_Yellow, outputLog.c_str()); break;
		case LogCategory::Error:	printf("%s%s\n", g_Red, outputLog.c_str()); break;
		default:					printf("%s\n", outputLog.c_str());
		}

		ResourceManager::Instance().GetFileManager()->WriteToLogFile(outputLog.c_str());
	}

	std::string Logger::GetLogCategoryName(const LogCategory _category)
	{
		switch (_category)
		{
		case LogCategory::Trace:	return STRINGIFY([TRACE]);
		case LogCategory::Info:		return STRINGIFY([INFO]);
		case LogCategory::Warning:	return STRINGIFY([WARNING]);
		case LogCategory::Error:	return STRINGIFY([ERROR]);
		default:					return "";
		}
	}
} // End of Banshee namespace