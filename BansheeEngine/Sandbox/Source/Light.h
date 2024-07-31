#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Components.h"
#include "Graphics/PrimitiveShape.h"

using namespace Banshee;

class Light
{
public:
	Light() : 
		m_Entity(EntityManager::CreateEntity()),
		m_Transform(m_Entity->AddComponent<TransformComponent>())
	{
		m_Entity->AddComponent<MeshComponent>(PrimitiveShape::CubeShape, ShaderType::Unlit);
		m_Entity->AddComponent<LightComponent>();
		m_Transform->SetPosition(glm::vec3(0.0f, 5.0f, 10.0f));
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};