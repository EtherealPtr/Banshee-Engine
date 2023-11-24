#pragma once

#include "Foundation/Platform.h"

namespace Banshee
{
	struct SortingLayer
	{
		SortingLayer(const uint32 _layerId, const int32 _priority) noexcept :
			layerId(_layerId),
			priority(_priority)
		{}

		uint32 layerId;
		int32 priority;
	};
} // End of Banshee namespace