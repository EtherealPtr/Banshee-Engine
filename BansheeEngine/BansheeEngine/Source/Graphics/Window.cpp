#include "Window.h"
#include "Foundation/Logging/Logger.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <assert.h>

namespace Banshee
{
	Window::Window(const uint16 _width, const uint16 _height, const std::string_view _title) :
		m_Window{ nullptr }
	{
		BE_LOG(LogCategory::Trace, "[WINDOW]: Creating window");

		if (!glfwInit())
		{
			throw std::runtime_error("ERROR: Failed to initialize glfw");
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		m_Window = glfwCreateWindow(_width, _height, _title.data(), nullptr, nullptr);
		if (!m_Window)
		{
			glfwTerminate();
			throw std::runtime_error("ERROR: Failed to create GLFW window");
		}

		BE_LOG(LogCategory::Info, "[WINDOW]: Created window");
	}

	Window::~Window()
	{
		if (m_Window)
		{
			glfwDestroyWindow(m_Window);
			m_Window = nullptr;
		}

		glfwTerminate();
	}

	bool Window::ShouldWindowClose() const noexcept
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::PollEvents() noexcept
	{
		glfwPollEvents();
	}

	uint16 Window::GetWidth() const noexcept
	{
		int w{ 0 };
		glfwGetFramebufferSize(m_Window, &w, nullptr);
		return w;
	}

	uint16 Window::GetHeight() const noexcept
	{
		int h{ 0 };
		glfwGetFramebufferSize(m_Window, nullptr, &h);
		return h;
	}
} // End of namespace