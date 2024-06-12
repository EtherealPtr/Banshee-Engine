#pragma once

#include "Foundation/Entity/EntityManager.h"
#include "Foundation/Components/MeshComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Graphics/PrimitiveShape.h"

using namespace Banshee;

class Player
{
public:
	Player()
	{
		m_PlayerEntity = EntityManager::Instance().CreateEntity();
		const std::shared_ptr<MeshComponent>& meshComponent = m_PlayerEntity->AddComponent<MeshComponent>();
		meshComponent->SetModel("Models/level1.obj");
		//meshComponent->SetColor(1.0f, 0.0f, 0.0f);
		m_Transform = m_PlayerEntity->GetTransform();

		//std::vector<InputBinding> inputBindings = 
		//{
		//   {BE_KEY_W, [&]() { MoveForward(); }},
		//   {BE_KEY_S, [&]() { MoveBackward(); }},
		//   {BE_KEY_A, [&]() { MoveLeft(); }},
		//   {BE_KEY_D, [&]() { MoveRight(); }}
		//};
		
		//Input::Instance().SetInputBindings(inputBindings);
	}

	/*void MoveForward()
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
	}*/

private:
	std::shared_ptr<Entity> m_PlayerEntity;
	std::shared_ptr<TransformComponent> m_Transform;
};