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
		const std::shared_ptr<MeshComponent>& meshComponent = m_Entity->AddComponent<MeshComponent>(PrimitiveShape::Cube, ShaderType::Standard, glm::vec3(1.0f, 1.0f, 1.0f));
		meshComponent->SetTexture("Textures/wood.jpg");
		m_Transform = m_Entity->AddComponent<TransformComponent>();
		m_Transform->SetPosition(glm::vec3(10.0f, 3.0f, 0.0f));
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};
