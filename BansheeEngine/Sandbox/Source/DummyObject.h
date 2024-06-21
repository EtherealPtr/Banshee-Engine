#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Components.h"
#include "Graphics/PrimitiveShape.h"

using namespace Banshee;

class DummyObjectOne
{
public:
	DummyObjectOne()
	{
		m_Entity = EntityManager::Instance().CreateEntity();
		const std::shared_ptr<MeshComponent>& meshComponent = m_Entity->AddComponent<MeshComponent>(PrimitiveShape::Triangle, glm::vec3(0.5f, 0.5f, 0.5f));
		meshComponent->SetTexture("Textures/wood.jpg");
		m_Transform = m_Entity->GetTransform();
		m_Transform.get()->SetPosition(glm::vec3(5.0f, 5.0f, 0.0f));
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};

class DummyObjectTwo
{
public:
	DummyObjectTwo()
	{
		m_Entity = EntityManager::Instance().CreateEntity();
		const std::shared_ptr<MeshComponent>& meshComponent = m_Entity->AddComponent<MeshComponent>("Models/texturetest.glb");
		m_Transform = m_Entity->GetTransform();
		m_Transform.get()->SetPosition(glm::vec3(0.0f, -10.0f, 0.0f));
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};