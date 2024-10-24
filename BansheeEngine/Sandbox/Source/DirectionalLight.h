#pragma once

#include "Foundation/Entity/Entity.h"
#include "Components.h"

using namespace Banshee;

class DirectionalLight : public Entity
{
public:
	DirectionalLight()
	{
		m_Transform = AddComponent<TransformComponent>();
		AddComponent<DirectionalLightComponent>(glm::vec3(0.5f, -0.2f, -1.0f));
	}

private:
	std::shared_ptr<TransformComponent> m_Transform;
};