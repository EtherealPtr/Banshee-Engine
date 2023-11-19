#pragma once

#include "Foundation/Platform.h"

struct GLFWwindow;

namespace Banshee
{
	class Window
	{
	public:
		Window(const uint16 _width, const uint16 _height, const char* _title);
		~Window();

		bool ShouldWindowClose() const noexcept;
		void Update() noexcept;
		GLFWwindow* GetWindow() const noexcept { return m_Window; }
		uint16 GetWidth() const noexcept;
		uint16 GetHeight() const noexcept;

	private:
		GLFWwindow* m_Window;
	};
} // End of Banshee namespace