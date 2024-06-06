#include "Input.h"
#include "GLFW/glfw3.h"

namespace Banshee
{
	Input& Input::Instance() noexcept
	{
		static Input s_InputSystemGLFW;
		return s_InputSystemGLFW;
	}

	void Input::Initialize(GLFWwindow* _window) noexcept
	{
		m_Window = _window;
	}

	void Input::LockCursor()
	{
		glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
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

		double mouseX, mouseY;
		GetCursorPosition(mouseX, mouseY);

		m_MouseXChange = mouseX - m_LastMouseX;
		m_MouseYChange = mouseY - m_LastMouseY;

		m_LastMouseX = mouseX;
		m_LastMouseY = mouseY;
	}

	bool Input::IsKeyPressed(int32 _key) const noexcept
	{
		return glfwGetKey(m_Window, _key) == GLFW_PRESS;
	}

	bool Input::IsMouseButtonPressed(int32 _button) const noexcept
	{
		return glfwGetMouseButton(m_Window, _button) == GLFW_PRESS;
	}

	void Input::GetCursorPosition(double& _x, double& _y) const noexcept
	{
		glfwGetCursorPos(m_Window, &_x, &_y);
	}

	void Input::SetInputBindings(const std::vector<InputBinding>& _bindings)
	{
		m_InputBindings = _bindings;
	}

	double Input::GetMouseXChange() const noexcept
	{
		return m_MouseXChange;
	}

	double Input::GetMouseYChange() const noexcept
	{
		return m_MouseYChange;
	}
} // End of Banshee namespace