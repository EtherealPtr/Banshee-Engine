#pragma once

#include "foundation/Platform.h"

struct GLFWwindow;

namespace Banshee
{
	class Window
	{
	public:
		Window(const uint16 _width, const uint16 _height, const char* _title);
		~Window();

		bool ShouldWindowClose() const;
		void Update();
		GLFWwindow* GetWindow() const { return m_Window; }
		uint32 GetWidth() const;
		uint32 GetHeight() const;

	private:
		GLFWwindow* m_Window;
	};

} // End of Banshee namespace
