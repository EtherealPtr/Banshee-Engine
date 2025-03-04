#include "Entity.h"
#include "Foundation/Entity/EntityManager.h"

namespace Banshee
{
	Entity::Entity() :
		m_Id{ EntityManager::RegisterEntity(this) },
		m_Components{}
	{}
} // End of namespace