#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Components.h"

using namespace Banshee;

class ExampleCustomModel
{
public:
	ExampleCustomModel() :
		m_Entity{ EntityManager::CreateEntity() },
		m_Transform{ m_Entity->AddComponent<TransformComponent>() }
	{
		auto meshComponent{ m_Entity->AddComponent<CustomMeshComponent>("Models/scene2.glb", ShaderType::Standard) };
		meshComponent->SetTintColor(glm::vec3(1.0f, 0.0f, 0.0f));
	}

	void SetPosition(const glm::vec3& _pos)
	{
		m_Transform->SetPosition(_pos);
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};

class ExampleSimpleModel
{
public:
	ExampleSimpleModel() :
		m_Entity{ EntityManager::CreateEntity() },
		m_Transform{ m_Entity->AddComponent<TransformComponent>() }
	{
		auto meshComponent{ m_Entity->AddComponent<SimpleMeshComponent>(PrimitiveShape::CubeShape, ShaderType::Standard) };
		meshComponent->SetTexture("Textures/tiles.jpg");
		m_Transform->SetPosition(glm::vec3(0.0f, 10.0f, 0.0f));
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};