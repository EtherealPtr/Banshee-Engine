#pragma once

#include "Foundation/Platform.h"
#include <chrono>

namespace Banshee
{
    class Timer 
    {
    public:
        Timer() noexcept : 
            m_LastTime(clock_t::now()),
            m_DeltaTime(0.0),
            m_ElapsedTime(0.0),
            m_FrameCount(0),
            m_Fps(0),
            m_FpsDisplayEnabled(false)
        {}

        void Update();
        void ToggleFpsDisplay() noexcept { m_FpsDisplayEnabled = !m_FpsDisplayEnabled; }
        double GetDeltaTime() const noexcept { return m_DeltaTime; }

    private:
        using clock_t = std::chrono::steady_clock;
        using time_point_t = std::chrono::time_point<clock_t>;

        time_point_t m_LastTime;
        double m_DeltaTime;
        double m_ElapsedTime;
        int32 m_FrameCount;
        int32 m_Fps;
        bool m_FpsDisplayEnabled;
    };
} // End of Banshee namespace
