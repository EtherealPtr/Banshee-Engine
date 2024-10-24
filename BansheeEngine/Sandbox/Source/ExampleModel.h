#pragma once

#include "Foundation/Entity/Entity.h"
#include "Components.h"

using namespace Banshee;

class ExampleCustomModel : public Entity
{
public:
	explicit ExampleCustomModel(const glm::vec3& _pos = glm::vec3(0.0f))
	{
		m_MeshComp = AddComponent<CustomMeshComponent>("Models/scene0.glb", ShaderType::Standard);
		m_Transform = AddComponent<TransformComponent>();
		m_Transform->SetPosition(_pos);
	}

	void SetTintColor(const glm::vec4& _col)
	{
		m_MeshComp->SetTintColor(_col);
	}

private:
	std::shared_ptr<TransformComponent> m_Transform;
	std::shared_ptr<CustomMeshComponent> m_MeshComp;
};

class ExamplePrimitiveModel : public Entity
{
public:
	explicit ExamplePrimitiveModel(const glm::vec3& _pos = glm::vec3(0.0f))
	{
		auto meshComponent{ AddComponent<PrimitiveMeshComponent>(PrimitiveShape::CubeShape, ShaderType::Standard) };
		meshComponent->SetTexture("Textures/tiles.jpg");
		m_Transform = AddComponent<TransformComponent>();
		m_Transform->SetPosition(_pos);
	}

private:
	std::shared_ptr<TransformComponent> m_Transform;
};