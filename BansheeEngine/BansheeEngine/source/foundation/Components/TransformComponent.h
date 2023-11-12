#pragma once

#include "Component.h"
#include "Foundation/DLLConfig.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Banshee
{
	class TransformComponent : public Component
	{
	public:
		BANSHEE_ENGINE TransformComponent() :
			m_Position(glm::vec3(0.0f, 0.0f, 0.0f)),
			m_Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f)),
			m_Scale(glm::vec3(1.0f))
		{}

		BANSHEE_ENGINE void SetPosition(const glm::vec3& _position);
		BANSHEE_ENGINE void SetRotation(const glm::quat& _rotation);
		BANSHEE_ENGINE void SetScale(const glm::vec3& _scale);

		BANSHEE_ENGINE void Translate(const glm::vec3& _translation);
		BANSHEE_ENGINE void Rotate(const glm::quat& _rotation);
		BANSHEE_ENGINE void Scale(const glm::vec3& _scale);

		BANSHEE_ENGINE glm::mat4 GetModel() const;
		BANSHEE_ENGINE const glm::vec3& GetPosition() const { return m_Position; }
		BANSHEE_ENGINE const glm::vec3& GetScale() const { return m_Scale; }

	private:
		glm::vec3 m_Position;
		glm::quat m_Rotation;
		glm::vec3 m_Scale;
	};
} // End of Banshee namespace