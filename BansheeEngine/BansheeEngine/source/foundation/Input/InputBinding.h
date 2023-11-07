#pragma once

#include "Foundation/Platform.h"
#include <functional>

namespace Banshee
{
	struct InputBinding
	{
		int32 key;
		std::function<void()> callback;
	};
} // End of Banshee namespace