#include "Entity.h"
#include "Foundation/Components/MeshComponent.h"
#include "Foundation/Systems/RenderSystem.h"

namespace Banshee
{
	Entity::Entity(const uint32 _id) :
		m_Id(_id),
		m_Components{}
	{}

	Entity::~Entity()
	{}
	
	void Entity::ApplyRenderableComponent(std::shared_ptr<MeshComponent>& _component)
	{
		RenderSystem::Instance().AddMeshComponent(_component);
	}
} // End of Banshee namespace