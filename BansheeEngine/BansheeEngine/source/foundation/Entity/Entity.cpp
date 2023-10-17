#include "Entity.h"
#include "Foundation/Components/MeshRendererComponent.h"
#include "Foundation/Systems/RenderSystem.h"

namespace Banshee
{
	Entity::Entity(const uint32 _id) :
		m_Id(_id),
		m_Components{}
	{}

	Entity::~Entity()
	{}
	
	void Entity::ApplyRenderableComponent(std::shared_ptr<MeshRendererComponent>& _component)
	{
		RenderSystem::Instance().AddMeshRendererComponent(_component);
	}
} // End of Banshee namespace