#pragma once

#include "LogCategories.h"
#include <string>

namespace Banshee
{
	class Logger
	{
	public:
		static Logger& Instance();

		void PrintLog(const LogCategory _category, const char* _format, ...);

	private:
		Logger() {};
		~Logger() {};

		Logger(const Logger&) = delete;
		Logger(Logger&&) = delete;
		void operator=(const Logger&) = delete;
		void operator=(Logger&&) = delete;

		std::string GetLogCategoryName(const LogCategory _category);
	};

#define BE_LOG(category, format, ...) Logger::Instance().PrintLog(category, format, __VA_ARGS__);

} // End of Banshee namespace