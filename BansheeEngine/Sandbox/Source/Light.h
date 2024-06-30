#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Components.h"
#include "Graphics/PrimitiveShape.h"

using namespace Banshee;

class Light
{
public:
	Light()
	{
		m_Entity = EntityManager::Instance().CreateEntity();
		m_Entity->AddComponent<MeshComponent>(PrimitiveShape::Cube, ShaderType::Unlit, glm::vec3(1.0f, 0.0f, 0.0f));
		m_Entity->AddComponent<LightComponent>(glm::vec3(1.0f, 0.0f, 0.0f));
		m_Transform = m_Entity->AddComponent<TransformComponent>();
		m_Transform->SetPosition(glm::vec3(0.0f, 5.0f, 0.0f));
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};