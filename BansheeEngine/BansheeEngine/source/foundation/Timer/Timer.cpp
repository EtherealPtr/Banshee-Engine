#include "Timer.h"
#include "Foundation/Logging/Logger.h"
#include <GLFW/glfw3.h>

namespace Banshee
{
	void Timer::Update()
	{
        const double currentTime = glfwGetTime();
        m_DeltaTime = currentTime - m_LastTime;
        m_LastTime = currentTime;

        if (!m_FpsDisplayEnabled)
            return;

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

	void Timer::ToggleFpsDisplay()
	{
		m_FpsDisplayEnabled = !m_FpsDisplayEnabled;
		if (!m_FpsDisplayEnabled) 
		{
			BE_LOG(LogCategory::Info, "FPS display disabled.");
		}
	}
} // End of Banshee namespace