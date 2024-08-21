#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Components.h"

using namespace Banshee;

class ExampleModel
{
public:
	ExampleModel() :
		m_Entity{ EntityManager::CreateEntity() },
		m_Transform{ m_Entity->AddComponent<TransformComponent>() }
	{
		m_Entity->AddComponent<MeshComponent>("Models/scene2.glb", ShaderType::Standard, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	void SetPosition(const glm::vec3& _pos)
	{
		m_Transform->SetPosition(_pos);
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};