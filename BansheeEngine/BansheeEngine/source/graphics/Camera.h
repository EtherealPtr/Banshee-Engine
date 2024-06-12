#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "MVP.h"

namespace Banshee
{
	class Camera
	{
	public:
		Camera(float _fov, float _aspect, float _near, float _far);

		const ViewProjMatrix& GetViewProjMatrix() const noexcept { return m_ViewProjMatrix; }
		const glm::mat4& GetViewMatrix() const noexcept { return m_ViewProjMatrix.view; }
		const glm::mat4& GetProjectionMatrix() const noexcept { return m_ViewProjMatrix.proj; }
		void ProcessInput();
		void MoveForward(float _delta);
		void MoveBackward(float _delta);
		void MoveRight(float _delta);
		void MoveLeft(float _delta);

	private:
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void ProcessKeyboardInput();
		void ProcessMouseInput();

	private:
		ViewProjMatrix m_ViewProjMatrix;

		glm::vec3 m_Position;
		glm::vec3 m_Up;
		glm::vec3 m_Right;
		glm::vec3 m_Front;

		float m_Fov;
		float m_Aspect;
		float m_Near;
		float m_Far;
		float m_Yaw;
		float m_Pitch;
	};
} // End of Banshee namespace