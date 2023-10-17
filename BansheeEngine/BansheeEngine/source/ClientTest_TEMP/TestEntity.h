#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Foundation/Components/MeshRendererComponent.h"

class Player
{
public:
	Player()
	{
		m_PlayerEntity = Banshee::EntityManager::Instance().CreateEntity();
		m_PlayerEntity->AddComponent<Banshee::MeshRendererComponent>();
	}

private:
	std::shared_ptr<Banshee::Entity> m_PlayerEntity;
};
