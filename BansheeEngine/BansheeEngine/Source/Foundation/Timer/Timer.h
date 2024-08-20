#pragma once

#include "Foundation/Platform.h"
#include <chrono>

namespace Banshee
{
	class Timer
	{
	public:
		Timer() noexcept;

		void Update();
		void ToggleFpsDisplay() noexcept { m_FpsDisplayEnabled = !m_FpsDisplayEnabled; }
		double GetDeltaTime() const noexcept { return m_DeltaTime; }
		void StartTimer() noexcept;
		void StopTimer();

		Timer(const Timer&) = delete;
		Timer(Timer&&) = delete;
		void operator=(const Timer&) = delete;
		void operator=(Timer&&) = delete;

	private:
		using clock_t = std::chrono::steady_clock;
		using time_point_t = std::chrono::time_point<clock_t>;

		time_point_t m_LastTime;
		time_point_t m_StartTime;
		double m_DeltaTime;
		double m_ElapsedTime;
		int32 m_FrameCount;
		int32 m_Fps;
		bool m_TimerRunning;
		bool m_FpsDisplayEnabled;
	};
} // End of Banshee namespace