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
		const std::shared_ptr<MeshComponent>& meshComponent = m_PlayerEntity->AddComponent<MeshComponent>("Models/texturetest.glb");
		//meshComponent->SetTexture("Textures/viking_room.png");
		m_Transform = m_PlayerEntity->GetTransform();
	}

private:
	std::shared_ptr<Entity> m_PlayerEntity;
	std::shared_ptr<TransformComponent> m_Transform;
};