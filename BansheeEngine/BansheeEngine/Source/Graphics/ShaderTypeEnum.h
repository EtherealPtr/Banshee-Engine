#pragma once

#include "Foundation/Platform.h"

namespace Banshee
{
    enum class ShaderTypeEnum : uint8
    {
        Standard,   // Phong lighting
        Unlit       // No lighting, just texture and color
    };
} // End of namespace
