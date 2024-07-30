#include "EntityManager.h"

namespace Banshee
{
	const EntityManager g_EntityManager{};

	std::shared_ptr<Entity> EntityManager::CreateEntity() const
	{
		auto entity = std::make_shared<Entity>(++m_TotalEntities);
		m_Entities.push_back(entity);
		return entity;
	}
} // End of Banshee namespace