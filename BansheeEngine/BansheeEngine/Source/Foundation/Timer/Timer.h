#pragma once

#include "Foundation/Platform.h"

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
        double m_LastTime;
        double m_DeltaTime;
        double m_ElapsedTime;
        int32 m_FrameCount;
        int32 m_Fps;
        bool m_FpsDisplayEnabled;
    };
} // End of Banshee namespace
