#pragma once

#include "Foundation/Platform.h"

namespace Banshee
{
	struct Image
	{
		unsigned char* pixels;
		int32 imageWidth;
		int32 imageHeight;
		uint64 imageSize;
		uint32 imageIndex;
	};
} // End of Banshee namespace