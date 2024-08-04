#pragma once

#include "Foundation/Platform.h"
#include <string>

struct GLFWwindow;

namespace Banshee
{
	class Window
	{
	public:
		Window(const uint16 _width, const uint16 _height, const std::string_view _title);
		~Window();

		bool ShouldWindowClose() const noexcept;
		void PollEvents() noexcept;
		GLFWwindow* GetWindow() const noexcept { return m_Window; }
		uint16 GetWidth() const noexcept;
		uint16 GetHeight() const noexcept;

		Window(const Window&) = delete;
		Window(Window&&) = delete;
		void operator=(const Window&) = delete;
		void operator=(Window&&) = delete;

	private:
		GLFWwindow* m_Window;
	};
} // End of Banshee namespace