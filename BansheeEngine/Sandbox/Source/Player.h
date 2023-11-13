#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Foundation/Components/MeshComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Graphics/PrimitiveShape.h"
#include "Application/Input/Input.h"

using namespace Banshee;

class Player
{
public:
	Player()
	{
		m_PlayerEntity = EntityManager::Instance().CreateEntity();
		auto meshComponent = m_PlayerEntity->AddComponent<MeshComponent>(PrimitiveShape::Triangle);
		m_Transform = m_PlayerEntity->GetTransform();
		m_Transform->SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		std::vector<InputBinding> inputBindings = 
		{
		   {BE_KEY_W, [&]() { MoveForward(); }},
		   {BE_KEY_S, [&]() { MoveBackward(); }},
		   {BE_KEY_A, [&]() { MoveLeft(); }},
		   {BE_KEY_D, [&]() { MoveRight(); }}
		};
		
		Input::Instance().SetInputBindings(inputBindings);
	}

	void MoveForward()
	{
		m_Transform->Translate(glm::vec3(0.0f, -0.001f, 0.0f));
	}

	void MoveBackward()
	{
		m_Transform->Translate(glm::vec3(0.0f, 0.001f, 0.0f));
	}

	void MoveLeft()
	{
		m_Transform->Translate(glm::vec3(-0.001f, 0.0f, 0.0f));
	}

	void MoveRight()
	{
		m_Transform->Translate(glm::vec3(0.001f, 0.0f, 0.0f));
	}

private:
	std::shared_ptr<Entity> m_PlayerEntity;
	std::shared_ptr<TransformComponent> m_Transform;
};