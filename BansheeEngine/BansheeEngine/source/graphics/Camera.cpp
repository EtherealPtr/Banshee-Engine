#include "Camera.h"
#include "Foundation/Input/Input.h"

namespace Banshee
{
	Camera::Camera(const float _fov, const float _aspect, const float _near, const float _far) :
		m_Fov(_fov), 
		m_Aspect(_aspect),
		m_Near(_near),
		m_Far(_far),
		m_Yaw(-90.0f),
		m_Pitch(0.0f)
	{
		Input::Instance().LockCursor();

		m_Position = glm::vec3(0.0f, 2.0f, 2.0f);
		m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
		m_Right = glm::vec3(1.0f, 0.0f, 0.0f);
		m_Front = glm::vec3(0.0f, 0.0f, -1.0f);

		UpdateViewMatrix();
		UpdateProjectionMatrix();
	}

	void Camera::ProcessInput(const double _deltaTime)
	{
		ProcessKeyboardInput(_deltaTime);
		ProcessMouseInput(_deltaTime);
	}

	void Camera::ProcessKeyboardInput(const double _deltaTime)
	{
		constexpr float baseSpeed = 15.0f;
		const float speed = baseSpeed * static_cast<float>(_deltaTime);

		if (Input::Instance().IsKeyPressed(BE_KEY_W))
		{
			m_Position += speed * m_Front;
		}
		if (Input::Instance().IsKeyPressed(BE_KEY_S))
		{
			m_Position -= speed * m_Front;
		}
		if (Input::Instance().IsKeyPressed(BE_KEY_A))
		{
			m_Position -= glm::normalize(glm::cross(m_Front, m_Up)) * speed;
		}
		if (Input::Instance().IsKeyPressed(BE_KEY_D))
		{
			m_Position += glm::normalize(glm::cross(m_Front, m_Up)) * speed;
		}

		UpdateViewMatrix();
	}

	void Camera::ProcessMouseInput(const double _deltaTime)
	{
		double xoffset = Input::Instance().GetMouseXChange();
		double yoffset = Input::Instance().GetMouseYChange();

		constexpr double baseSensitivity = 25.0;
		const double sensitivity = baseSensitivity * _deltaTime;
		xoffset *= sensitivity;
		yoffset *= sensitivity;

		m_Yaw += static_cast<float>(xoffset);
		m_Pitch -= static_cast<float>(yoffset);

		if (m_Pitch > 89.0f)
			m_Pitch = 89.0f;
		if (m_Pitch < -89.0f)
			m_Pitch = -89.0f;

		UpdateViewMatrix();
	}

	void Camera::UpdateViewMatrix()
	{
		glm::vec3 front{};
		front.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		front.y = sin(glm::radians(m_Pitch));
		front.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		m_Front = glm::normalize(front);

		m_ViewProjMatrix.view = glm::lookAt(m_Position, m_Position + m_Front, m_Up);
	}

	void Camera::UpdateProjectionMatrix()
	{
		m_ViewProjMatrix.proj = glm::perspective(glm::radians(m_Fov), m_Aspect, m_Near, m_Far);
	}
} // End of Banshee namespace