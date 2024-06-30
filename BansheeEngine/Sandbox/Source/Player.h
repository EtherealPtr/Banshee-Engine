#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Components.h"

using namespace Banshee;

class Player
{
public:
	Player()
	{
		m_PlayerEntity = EntityManager::Instance().CreateEntity();
		m_PlayerEntity->AddComponent<MeshComponent>("Models/scene0.glb");
	}

private:
	std::shared_ptr<Entity> m_PlayerEntity;
};