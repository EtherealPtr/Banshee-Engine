#pragma once

#include "Foundation/DLLConfig.h"
#include "InputSystem.h"
#include "InputMapping.h"

namespace Banshee
{
	class Input : public InputSystem
	{
	public:
		BANSHEE_ENGINE static Input& Instance() noexcept;

		virtual void Initialize(GLFWwindow* _window) noexcept override;
		virtual void Update() override;
		virtual bool IsKeyPressed(const int32 _key) const noexcept override;
		virtual bool IsMouseButtonPressed(const int32 _button) const noexcept override;
		virtual void GetCursorPosition(double& _x, double& _y) const noexcept override;
		virtual void SetInputBindings(const std::vector<InputBinding>& _bindings) override { m_InputBindings = _bindings; }
		virtual double GetMouseXChange() const noexcept override { return m_MouseXChange; }
		virtual double GetMouseYChange() const noexcept override { return m_MouseYChange; }

		void LockCursor() noexcept;

		Input(const Input&) = delete;
		Input(Input&&) = delete;
		void operator=(const Input&) = delete;
		void operator=(Input&&) = delete;

	private:
		Input() noexcept;

	private:
		GLFWwindow* m_Window;
		std::vector<InputBinding> m_InputBindings;

		double m_MouseXChange;
		double m_MouseYChange;
		double m_LastMouseX;
		double m_LastMouseY;
	};
} // End of Banshee namespace