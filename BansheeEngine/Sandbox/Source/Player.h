#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Components.h"

using namespace Banshee;

class Player
{
public:
	Player() : 
		m_Entity(EntityManager::Instance().CreateEntity()),
		m_Transform(m_Entity->AddComponent<TransformComponent>())
	{
		m_Entity->AddComponent<MeshComponent>("Models/scene2.glb");
		m_Transform->SetPosition(glm::vec3(0.0f, 0.0f, -1.0f));
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};