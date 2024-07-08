#include "TransformComponent.h"
#include <glm/gtc/matrix_transform.hpp>

namespace Banshee
{
	void TransformComponent::SetPosition(const glm::vec3& _position)
	{
		m_Position = _position;
		SetDirty(true);
	}

	void TransformComponent::SetRotation(const glm::quat& _rotation)
	{
		m_Rotation = _rotation;
		SetDirty(true);
	}

	void TransformComponent::SetScale(const glm::vec3& _scale)
	{
		m_Scale = _scale;
		SetDirty(true);
	}

	void TransformComponent::Translate(const glm::vec3& _translation)
	{
		m_Position += _translation;
		SetDirty(true);
	}

	void TransformComponent::Rotate(const glm::quat& _rotation)
	{
		m_Rotation *= _rotation;
		SetDirty(true);
	}

	void TransformComponent::Scale(const glm::vec3& _scale)
	{
		m_Scale *= _scale;
		SetDirty(true);
	}

	glm::mat4 TransformComponent::GetModel() const
	{
		const glm::mat4 translationMatrix = glm::translate(glm::mat4(1.0f), m_Position);
		const glm::mat4 rotationMatrix = glm::mat4_cast(m_Rotation);
		const glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_Scale);
		return translationMatrix * rotationMatrix * scaleMatrix;
	}
} // End of Banshee namespace