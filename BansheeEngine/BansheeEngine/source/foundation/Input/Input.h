#pragma once

#include "InputSystem.h"
#include "InputMapping.h"

namespace Banshee
{
	class Input : public InputSystem
	{
	public:
		static Input& Instance() noexcept;

		virtual void Initialize(GLFWwindow* _window) noexcept override;
		virtual void Update() override;
		virtual bool IsKeyPressed(const int32 _key) const noexcept override;
		virtual bool IsMouseButtonPressed(const int32 _button) const noexcept override;
		virtual void GetCursorPosition(double& _x, double& _y) const noexcept override;
		virtual void SetInputBindings(const std::vector<InputBinding>& _bindings) override;
		virtual double GetMouseXChange() const noexcept override;
		virtual double GetMouseYChange() const noexcept override;

		void LockCursor();

		Input(const Input&) = delete;
		Input(Input&&) = delete;
		void operator=(const Input&) = delete;
		void operator=(Input&&) = delete;

	private:
		Input() noexcept;
		virtual ~Input() = default;

	private:
		GLFWwindow* m_Window;
		std::vector<InputBinding> m_InputBindings;

		double m_MouseXChange;
		double m_MouseYChange;
		double m_LastMouseX;
		double m_LastMouseY;
	};
} // End of Banshee namespace