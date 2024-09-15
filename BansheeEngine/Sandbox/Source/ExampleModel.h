#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Components.h"

using namespace Banshee;

class ExampleCustomModel
{
public:
	explicit ExampleCustomModel(const glm::vec3& _pos = glm::vec3(0.0f)) :
		m_Entity{ EntityManager::CreateEntity() },
		m_Transform{ m_Entity->AddComponent<TransformComponent>() }
	{
		m_MeshComp = m_Entity->AddComponent<CustomMeshComponent>("Models/scene0.glb", ShaderType::Standard);
		m_Transform->SetPosition(_pos);
	}

	void SetTintColor(const glm::vec3& _col)
	{
		m_MeshComp->SetTintColor(_col);
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
	std::shared_ptr<CustomMeshComponent> m_MeshComp;
};

class ExamplePrimitiveModel
{
public:
	explicit ExamplePrimitiveModel(const glm::vec3& _pos = glm::vec3(0.0f)) :
		m_Entity{ EntityManager::CreateEntity() },
		m_Transform{ m_Entity->AddComponent<TransformComponent>() }
	{
		auto meshComponent{ m_Entity->AddComponent<PrimitiveMeshComponent>(PrimitiveShape::CubeShape, ShaderType::Standard) };
		meshComponent->SetTexture("Textures/tiles.jpg");
		m_Transform->SetPosition(_pos);
	}

private:
	std::shared_ptr<Entity> m_Entity;
	std::shared_ptr<TransformComponent> m_Transform;
};