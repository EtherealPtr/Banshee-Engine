#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Components.h"
#include "Graphics/PrimitiveShape.h"

using namespace Banshee;

class DummyObjectOne
{
public:
	DummyObjectOne() : 
		m_Entity(EntityManager::CreateEntity()),
		m_Transform(m_Entity->AddComponent<TransformComponent>())
	{
		const auto& meshComponent = m_Entity->AddComponent<MeshComponent>(PrimitiveShape::CubeShape, ShaderType::Standard, glm::vec3(1.0f, 1.0f, 1.0f));
		meshComponent->SetTexture("Textures/wood.jpg");
		m_Transform->SetPosition(glm::vec3(0.0f, 2.0f, -4.0f));
		m_Transform->SetScale(glm::vec3(2.0f));
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};
