#include "Window.h"
#include "foundation/Logger.h"
#include <glfw3.h>
#include <stdexcept>
#include <assert.h>

namespace Banshee
{
	Window::Window(const uint16 _width, const uint16 _height, const char* _title) :
		m_Window(nullptr)
	{
		BE_LOG(LogCategory::trace, "Window creation stage: Start");

		if (!glfwInit())
		{
			BE_LOG(LogCategory::error, "Failed to initialize glfw");
			throw std::runtime_error("ERROR: Failed to initialize glfw");
		}

		BE_LOG(LogCategory::info, "GLFW initialized successfully");

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		m_Window = glfwCreateWindow(_width, _height, _title, nullptr, nullptr);
		assert(m_Window != nullptr);

		BE_LOG(LogCategory::trace, "Window creation stage: End");
	}

	Window::~Window()
	{
		glfwDestroyWindow(m_Window);
		m_Window = nullptr;
		BE_LOG(LogCategory::trace, "GLFW window destroyed");

		glfwTerminate();
		BE_LOG(LogCategory::trace, "GLFW terminated");
	}
	
	bool Window::ShouldWindowClose() const
	{
		return glfwWindowShouldClose(m_Window);
	}
	
	void Window::Update()
	{
		glfwPollEvents();
	}

	uint32 Window::GetWidth() const
	{
		int32 w = 0;
		glfwGetFramebufferSize(m_Window, &w, nullptr);
		return static_cast<uint32>(w);
	}

	uint32 Window::GetHeight() const
	{
		int32 h = 0;
		glfwGetFramebufferSize(m_Window, nullptr, &h);
		return static_cast<uint32>(h);
	}
} // End of Banshee namespace