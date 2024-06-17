#pragma once

#include "Entity.h"

namespace Banshee
{
	class EntityManager
	{
	public:
		BANSHEE_ENGINE static EntityManager& Instance() noexcept;
		BANSHEE_ENGINE static std::shared_ptr<Entity> CreateEntity();
		const static std::vector<std::shared_ptr<Entity>>& GetAllEntities() noexcept { return m_Entities; }

		EntityManager(const EntityManager&) = delete;
		EntityManager(EntityManager&&) = delete;
		void operator=(const EntityManager&) = delete;
		void operator=(EntityManager&&) = delete;

	private:
		EntityManager() noexcept {};
		~EntityManager() {};

	private:
		static EntityManager s_EntityManager;
		static uint32 m_TotalEntities;
		static std::vector<std::shared_ptr<Entity>> m_Entities;
	};
} // End of Banshee namespace