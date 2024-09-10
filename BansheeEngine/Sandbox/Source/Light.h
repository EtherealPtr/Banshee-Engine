#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Graphics/PrimitiveShape.h"
#include "Components.h"

using namespace Banshee;

class Light
{
public:
	Light() : 
		m_Entity{ EntityManager::CreateEntity() },
		m_Transform{ m_Entity->AddComponent<TransformComponent>() }
	{
		m_Entity->AddComponent<PrimitiveMeshComponent>(PrimitiveShape::CubeShape, ShaderType::Unlit);
		m_Entity->AddComponent<LightComponent>();
		m_Transform->SetPosition(glm::vec3(-1.0f, 1.0f, 0.0f));
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};