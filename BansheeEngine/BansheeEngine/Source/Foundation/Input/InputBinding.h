#pragma once

#include "Foundation/Platform.h"
#include <functional>

namespace Banshee
{
	struct InputBinding
	{
		int32 m_Key;
		std::function<void()> m_Callback;
	};
} // End of Banshee namespace