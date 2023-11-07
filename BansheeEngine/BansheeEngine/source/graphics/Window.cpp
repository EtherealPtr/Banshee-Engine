#include "Window.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <assert.h>

namespace Banshee
{
	Window::Window(const uint16_t _width, const uint16_t _height, const char* _title) :
		m_Window(nullptr)
	{
		if (!glfwInit())
		{
			throw std::runtime_error("ERROR: Failed to initialize glfw");
		}

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

		m_Window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
		assert(m_Window != nullptr);
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		m_Window = nullptr;

		glfwTerminate();
	}

	bool Window::ShouldWindowClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::Update()
	{
		glfwPollEvents();
	}

	uint16_t Window::GetWidth() const
	{
		int w = 0;
		glfwGetFramebufferSize(m_Window, &w, nullptr);
		return static_cast<uint16_t>(w);
	}

	uint16_t Window::GetHeight() const
	{
		int h = 0;
		glfwGetFramebufferSize(m_Window, nullptr, &h);
		return static_cast<uint16_t>(h);
	}
} // End of Banshee namespace