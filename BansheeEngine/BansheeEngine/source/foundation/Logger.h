#pragma once

#include "Platform.h"
#include "Subsystem.h"
#include <string>

namespace Banshee
{
	enum class LogCategory : uint8
	{
		trace,
		info,
		warning,
		error
	};

	class Logger : public Subsystem
	{
	public:
		DECLARE_BANSHEE_SYSTEM(Logger);

		void PrintLog(const LogCategory _category, const char* _format, ...);

	private:
		std::string GetLogCategoryName(const LogCategory _category);
	};

#define BE_LOG(category, format, ...) Logger::Instance().PrintLog(category, format, __VA_ARGS__);

} // End of Banshee namespace