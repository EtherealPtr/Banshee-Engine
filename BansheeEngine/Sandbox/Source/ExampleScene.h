#pragma once

#include "Foundation/Entity/Entity.h"
#include "Components.h"

using namespace Banshee;

class ExampleCustomModel : public Entity
{
public:
	explicit ExampleCustomModel(const glm::vec3& _pos = glm::vec3(0.0f))
	{
		m_MeshComp = AddComponent<CustomMeshComponent>("Models/monkey.glb", ShaderTypeEnum::Standard);
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

class ExampleCube : public Entity
{
public:
	ExampleCube(const glm::vec3& _pos = glm::vec3(0.0f), const glm::vec3& _tintColor = glm::vec3(1.0f), const char* _texturePath = nullptr)
	{
		auto meshComponent{ AddComponent<PrimitiveMeshComponent>(PrimitiveShapeEnum::CubeShape, ShaderTypeEnum::Standard) };

		if (_texturePath)
		{
			meshComponent->SetTexture(_texturePath);
		}
		else
		{
			meshComponent->SetTintColor(glm::vec4(_tintColor.x, _tintColor.y, _tintColor.z, 1.0f));
		}

		m_Transform = AddComponent<TransformComponent>();
		m_Transform->SetPosition(_pos);
	}

	void SetPosition(const glm::vec3& _pos)
	{
		m_Transform->SetPosition(_pos);
	}

	void SetScale(const float _scale)
	{
		m_Transform->SetScale(glm::vec3(_scale));
	}

private:
	std::shared_ptr<TransformComponent> m_Transform;
};

class ExampleQuad : public Entity
{
public:
	explicit ExampleQuad(const glm::vec3& _pos = glm::vec3(0.0f))
	{
		auto meshComponent{ AddComponent<PrimitiveMeshComponent>(PrimitiveShapeEnum::SquareShape, ShaderTypeEnum::Standard) };
		meshComponent->SetTintColor(glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
		m_Transform = AddComponent<TransformComponent>();
		m_Transform->SetPosition(_pos);
		m_Transform->SetScale(glm::vec3(10.0f, 10.0f, 1.0f));
		m_Transform->SetRotation(glm::quat(glm::vec3(glm::radians(-90.0f), 0.0f, 0.0f)));
	}

private:
	std::shared_ptr<TransformComponent> m_Transform;
};