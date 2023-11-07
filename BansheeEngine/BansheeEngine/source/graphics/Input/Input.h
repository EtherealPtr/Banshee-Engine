#pragma once

#include "Foundation/Input/InputSystem.h"
#include "Foundation/DLLConfig.h"

namespace Banshee
{
	class Input : public InputSystem
	{
	public:
		BANSHEE_ENGINE static Input& Instance();

		virtual void Initialize(GLFWwindow* _window) override;
		virtual void Update() override;
		virtual bool IsKeyPressed(int32 _key) const override;
		virtual bool IsMouseButtonPressed(int32 _button) const override;
		virtual void GetCursorPosition(double& _x, double& _y) const override;
		virtual void SetInputBindings(const std::vector<InputBinding>& _bindings) override;

	private:
		Input() : m_Window(nullptr) {}
		~Input() = default;

		Input(const Input&) = delete;
		Input(Input&&) = delete;
		void operator=(const Input&) = delete;
		void operator=(Input&&) = delete;

	private:
		GLFWwindow* m_Window;
		std::vector<InputBinding> m_InputBindings;
	};
}