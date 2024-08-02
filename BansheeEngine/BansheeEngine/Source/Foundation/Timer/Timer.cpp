#include "Timer.h"
#include "Foundation/Logging/Logger.h"

namespace Banshee
{
	void Timer::Update()
	{
        const auto currentTime = clock_t::now();
        const std::chrono::duration<double> delta = currentTime - m_LastTime;
        m_DeltaTime = delta.count();
        m_LastTime = currentTime;

        if (!m_FpsDisplayEnabled)
        {
            return;
        }

        ++m_FrameCount;
        m_ElapsedTime += m_DeltaTime;

        if (m_ElapsedTime >= 1.0)
        {
            m_Fps = m_FrameCount;
            m_FrameCount = 0;
            m_ElapsedTime = 0.0;
            BE_LOG(LogCategory::Trace, "FPS: %d", m_Fps);
        }
	}

    void Timer::StartTimer() noexcept
    {
        m_StartTime = clock_t::now();
    }

    void Timer::StopTimer() const
    {
        const auto elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(clock_t::now() - m_StartTime).count();
        BE_LOG(LogCategory::Trace, "Elapsed time: %d", elapsedTime);
    }
} // End of Banshee namespace