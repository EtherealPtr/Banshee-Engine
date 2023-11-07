#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Foundation/Components/MeshRendererComponent.h"

using namespace Banshee;

class Player
{
public:
	Player()
	{
		m_PlayerEntity = EntityManager::Instance().CreateEntity();
		m_PlayerEntity->AddComponent<MeshRendererComponent>();

		//std::vector<Banshee::InputBinding> inputBindings = {
		//   {32, [&]() { printf("HELLO WORLD"); }},
		//};
		//
		//Banshee::Input::Instance().SetInputBindings(inputBindings);
	}

private:
	std::shared_ptr<Entity> m_PlayerEntity;
};
