#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Foundation/Components/MeshRendererComponent.h"
#include "Application/Input/InputKeys.h"
#include "Application/Input/Input.h"

using namespace Banshee;

class Player
{
public:
	Player()
	{
		m_PlayerEntity = EntityManager::Instance().CreateEntity();
		m_PlayerEntity->AddComponent<MeshRendererComponent>();

		std::vector<Banshee::InputBinding> inputBindings = 
		{
		   {BE_KEY_W, [&]() { MoveForward(); }},
		   {BE_KEY_S, [&]() { MoveBackward(); }},
		   {BE_KEY_A, [&]() { MoveLeft(); }},
		   {BE_KEY_D, [&]() { MoveRight(); }}
		};
		
		Banshee::Input::Instance().SetInputBindings(inputBindings);
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
