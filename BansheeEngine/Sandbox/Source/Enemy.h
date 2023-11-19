#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Foundation/Components/MeshComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Graphics/PrimitiveShape.h"

using namespace Banshee;

class Enemy
{
public:
	Enemy()
	{
		m_EnemyEntity = EntityManager::Instance().CreateEntity();
		const std::shared_ptr<MeshComponent>& meshComponent = m_EnemyEntity->AddComponent<MeshComponent>(PrimitiveShape::Triangle);
		meshComponent->SetTexture("Textures/gengar.png");
		m_Transform = m_EnemyEntity->GetTransform();
		m_Transform->SetPosition(glm::vec3(0.5f, 0.0f, 0.0f));
	}

private:
	std::shared_ptr<Entity> m_EnemyEntity;
	std::shared_ptr<TransformComponent> m_Transform;
};