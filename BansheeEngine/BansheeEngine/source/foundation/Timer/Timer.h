#pragma once

#include "Foundation/Platform.h"
#include <chrono>

namespace Banshee
{
    class Timer 
    {
    public:
        Timer() noexcept : 
            m_LastTime(0),
            m_DeltaTime(0.0),
            m_ElapsedTime(0.0),
            m_FrameCount(0),
            m_Fps(0),
            m_FpsDisplayEnabled(false)
        {}

        void Update();
        void ToggleFpsDisplay();
        double GetDeltaTime() const noexcept { return m_DeltaTime; }

    private:
        double m_LastTime = 0.0;
        double m_DeltaTime = 0.0;
        double m_ElapsedTime = 0.0;
        int32 m_FrameCount = 0;
        int32 m_Fps = 0;
        bool m_FpsDisplayEnabled = false;
    };
} // End of Banshee namespace
