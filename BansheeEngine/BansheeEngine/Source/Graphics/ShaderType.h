#pragma once

#include "Foundation/Platform.h"

namespace Banshee
{
    enum class ShaderType : uint8
    {
        Standard,   // Phong lighting
        Unlit       // No lighting, just texture and color
    };
} // End of Banshee namespace
