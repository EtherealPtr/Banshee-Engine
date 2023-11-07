#include "Input.h"
#include "GLFW/glfw3.h"

namespace Banshee
{
	Input& Input::Instance()
	{
		static Input s_InputSystemGLFW;
		return s_InputSystemGLFW;
	}

	void Input::Initialize(GLFWwindow* _window)
	{
		m_Window = _window;
	}

	void Input::Update()
	{
		for (const auto& binding : m_InputBindings)
		{
			if (IsKeyPressed(binding.key))
			{
				binding.callback();
			}
		}
	}

	bool Input::IsKeyPressed(int32 _key) const
	{
		return glfwGetKey(m_Window, _key) == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(int32 _button) const
	{
		return glfwGetMouseButton(m_Window, _button) == GLFW_PRESS;
	}

	void Input::GetCursorPosition(double& _x, double& _y) const
	{
		glfwGetCursorPos(m_Window, &_x, &_y);
	}

	void Input::SetInputBindings(const std::vector<InputBinding>& _bindings)
	{
		m_InputBindings = _bindings;
	}
}
