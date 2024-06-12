#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Foundation/Components/MeshComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Graphics/PrimitiveShape.h"

using namespace Banshee;

class Ground
{
public:
	Ground()
	{
		m_Entity = EntityManager::Instance().CreateEntity();
		const std::shared_ptr<MeshComponent>& meshComponent = m_Entity->AddComponent<MeshComponent>(PrimitiveShape::Square);
		meshComponent->SetTexture("Textures/tiles.jpg");
		m_Transform = m_Entity->GetTransform();
		m_Transform.get()->SetRotation(glm::angleAxis(glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f)));
		m_Transform.get()->SetScale(glm::vec3(5.0f, 5.0f, 1.0f));
		m_Transform.get()->SetPosition(glm::vec3(0.0f, -1.0f, 0.0f));
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};

class Ground2
{
public:
	Ground2()
	{
		m_Entity = EntityManager::Instance().CreateEntity();
		const std::shared_ptr<MeshComponent>& meshComponent = m_Entity->AddComponent<MeshComponent>(PrimitiveShape::Triangle);
		meshComponent->GetMaterials()[0].SetDiffuseColor({0.0f, 1.0f, 0.0f});
		m_Transform = m_Entity->GetTransform();
		m_Transform.get()->SetScale(glm::vec3(3.0f, 3.0f, 1.0f));
		m_Transform.get()->SetPosition(glm::vec3(-5.0f, 0.0f, 0.0f));
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};