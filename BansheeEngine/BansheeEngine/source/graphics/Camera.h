#pragma once

#include "MVP.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Banshee
{
	class Camera
	{
	public:
		Camera(const float _fov, const float _aspect, const float _near, const float _far);

		const ViewProjMatrix& GetViewProjMatrix() const noexcept { return m_ViewProjMatrix; }
		const glm::mat4& GetViewMatrix() const noexcept { return m_ViewProjMatrix.view; }
		const glm::mat4& GetProjectionMatrix() const noexcept { return m_ViewProjMatrix.proj; }
		void ProcessInput(const double _deltaTime);

	private:
		void UpdateViewMatrix();
		void UpdateProjectionMatrix();
		void ProcessKeyboardInput(const double _deltaTime);
		void ProcessMouseInput(const double _deltaTime);

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