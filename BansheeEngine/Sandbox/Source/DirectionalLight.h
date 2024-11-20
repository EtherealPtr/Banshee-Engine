#pragma once

#include "Foundation/Entity/Entity.h"
#include "Components.h"

using namespace Banshee;

class DirectionalLight : public Entity
{
public:
	DirectionalLight(const glm::vec3& _direction = glm::vec3(0.5f, -0.2f, -1.0f), const glm::vec4& _color = glm::vec4(1.0f))
	{
		m_Transform = AddComponent<TransformComponent>();
		AddComponent<DirectionalLightComponent>(_direction, _color);
	}

private:
	std::shared_ptr<TransformComponent> m_Transform;
};