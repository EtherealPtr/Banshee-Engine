#include "Entity.h"
#include "Foundation/Components/TransformComponent.h"
#include "Foundation/Logging/Logger.h"

namespace Banshee
{
	Entity::Entity(const uint32 _id) :
		m_Id(_id),
		m_Components{}
	{
		m_Transform = AddComponent<TransformComponent>();
	}

	void Entity::RegisterComponent(const std::shared_ptr<Component>& _component) const
	{
		if (!_component)
		{
			BE_LOG(LogCategory::Warning, "[ENTITY]: Attempted to add an invalid component");
			return;
		}

		_component->SetOwner(this);
	}
} // End of Banshee namespace