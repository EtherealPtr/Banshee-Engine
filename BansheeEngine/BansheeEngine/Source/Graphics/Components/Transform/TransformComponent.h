#pragma once

#include "Foundation/DLLConfig.h"
#include "Foundation/Components/Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

namespace Banshee
{
	class TransformComponent : public Component
	{
	public:
		BANSHEE_ENGINE TransformComponent() noexcept :
			m_Position{ glm::vec3(0.0f, 0.0f, 0.0f) },
			m_Scale{ glm::vec3(1.0f) },
			m_Rotation{ glm::quat(1.0f, 0.0f, 0.0f, 0.0f) }
		{}

		BANSHEE_ENGINE void SetPosition(const glm::vec3& _position) noexcept;
		BANSHEE_ENGINE void SetRotation(const glm::quat& _rotation) noexcept;
		BANSHEE_ENGINE void SetScale(const glm::vec3& _scale) noexcept;
		BANSHEE_ENGINE void Translate(const glm::vec3& _translation) noexcept;
		BANSHEE_ENGINE void Rotate(const glm::quat& _rotation) noexcept;
		BANSHEE_ENGINE void Scale(const glm::vec3& _scale) noexcept;
		BANSHEE_ENGINE glm::mat4 GetModel() const;
		BANSHEE_ENGINE const glm::vec3& GetPosition() const noexcept { return m_Position; }
		BANSHEE_ENGINE const glm::vec3& GetScale() const noexcept { return m_Scale; }

	private:
		glm::vec3 m_Position;
		glm::vec3 m_Scale;
		glm::quat m_Rotation;
	};
} // End of Banshee namespace