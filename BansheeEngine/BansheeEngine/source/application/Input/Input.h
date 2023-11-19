#pragma once

#include "Foundation/Input/InputSystem.h"
#include "Foundation/DLLConfig.h"
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
		virtual void SetInputBindings(const std::vector<InputBinding>& _bindings) override;

	private:
		Input() noexcept : m_Window(nullptr) {}
		virtual ~Input() = default;

		Input(const Input&) = delete;
		Input(Input&&) = delete;
		void operator=(const Input&) = delete;
		void operator=(Input&&) = delete;

	private:
		GLFWwindow* m_Window;
		std::vector<InputBinding> m_InputBindings;
	};
} // End of Banshee namespace