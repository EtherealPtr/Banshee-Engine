#pragma once

#include "Foundation/Platform.h"

struct GLFWwindow;

namespace Banshee
{
	class Window
	{
	public:
		Window(const uint16_t _width, const uint16_t _height, const char* _title);
		~Window();

		bool ShouldWindowClose() const;
		void Update();
		GLFWwindow* GetWindow() const { return m_Window; }
		uint16_t GetWidth() const;
		uint16_t GetHeight() const;

	private:
		GLFWwindow* m_Window;
	};
}
