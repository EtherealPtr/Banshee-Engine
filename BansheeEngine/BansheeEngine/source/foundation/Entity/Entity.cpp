#include "Entity.h"
#include "Foundation/Components/MeshComponent.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Systems/RenderSystem.h"
#include "Foundation/Logging/Logger.h"

namespace Banshee
{
	Entity::Entity(const uint32 _id) :
		m_Id(_id),
		m_Components{}
	{
		m_Transform = AddComponent<TransformComponent>();
	}
	
	void Entity::ApplyRenderableComponent(const std::shared_ptr<MeshComponent>& _component)
	{
		RenderSystem::Instance().AddMeshComponent(_component);
	}

	void Entity::RegisterComponent(Component* const _component)
	{
		if (!_component)
		{
			BE_LOG(LogCategory::Warning, "Attempted to add an invalid component");
			return;
		}

		_component->SetOwner(this);
	}
} // End of Banshee namespace