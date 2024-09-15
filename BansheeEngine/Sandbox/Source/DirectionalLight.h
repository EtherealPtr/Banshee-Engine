#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Graphics/PrimitiveShape.h"
#include "Components.h"

using namespace Banshee;

class DirectionalLight
{
public:
	DirectionalLight() :
		m_Entity{ EntityManager::CreateEntity() },
		m_Transform{ m_Entity->AddComponent<TransformComponent>() }
	{
		m_Entity->AddComponent<PrimitiveMeshComponent>(PrimitiveShape::CubeShape, ShaderType::Unlit);
		m_Entity->AddComponent<DirectionalLightComponent>(glm::vec3(0.5f, -0.2f, -1.0f));
		m_Transform->SetPosition(glm::vec3(0.0f, 10.0f, 0.0f)); // Position is unnecessary for directional light but it's nice to see something for it anyway
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};