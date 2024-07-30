#pragma once

#include "Entity.h"

namespace Banshee
{
	class EntityManager
	{
	public:
		BANSHEE_ENGINE EntityManager() noexcept :
			m_TotalEntities{ 0 },
			m_Entities{}
		{}
		BANSHEE_ENGINE ~EntityManager() = default;

		BANSHEE_ENGINE std::shared_ptr<Entity> CreateEntity() const;
		const std::vector<std::shared_ptr<Entity>>& GetAllEntities() const noexcept { return m_Entities; }

		EntityManager(const EntityManager&) = delete;
		EntityManager(EntityManager&&) = delete;
		void operator=(const EntityManager&) = delete;
		void operator=(EntityManager&&) = delete;

	private:
		mutable uint32 m_TotalEntities;
		mutable std::vector<std::shared_ptr<Entity>> m_Entities;
	};

	BANSHEE_ENGINE extern const EntityManager g_EntityManager;
} // End of Banshee namespace