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
		auto transform = m_PlayerEntity->GetComponent<TransformComponent>();
		transform.get()->SetPosition(glm::vec3(0.0f, -0.5f, 0.0f));

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
		printf("Move forward\n");
	}

	void MoveBackward()
	{
		printf("Move backward\n");
	}

	void MoveLeft()
	{
		printf("Move left\n");
	}

	void MoveRight()
	{
		printf("Move right\n");
	}

private:
	std::shared_ptr<Entity> m_PlayerEntity;
};