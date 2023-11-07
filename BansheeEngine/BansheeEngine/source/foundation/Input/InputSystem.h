#pragma once

#include "InputBinding.h"

struct GLFWwindow;

namespace Banshee
{
	class InputSystem
	{
	public:
		virtual ~InputSystem() = default;

		virtual void Initialize(GLFWwindow* _window) = 0;
		virtual void Update() = 0;
		virtual void GetCursorPosition(double& _x, double& _y) const = 0;
		virtual void SetInputBindings(const std::vector<InputBinding>& _bindings) = 0;
		virtual bool IsKeyPressed(int32 _key) const = 0;
		virtual bool IsMouseButtonPressed(int32 _button) const = 0;
	};
} // End of Banshee namespace