#include "Entity.h"
#include "Foundation/Components/MeshComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Systems/RenderSystem.h"

namespace Banshee
{
	Entity::Entity(const uint32 _id) :
		m_Id(_id),
		m_Components{}
	{
		m_Transform = AddComponent<TransformComponent>();
	}

	Entity::~Entity()
	{}
	
	void Entity::ApplyRenderableComponent(std::shared_ptr<MeshComponent>& _component)
	{
		RenderSystem::Instance().AddMeshComponent(_component);
	}

	void Entity::RegisterComponent(Component* _component)
	{
		_component->SetOwner(this);
	}
} // End of Banshee namespace