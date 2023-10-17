#pragma once

#include "Foundation/DLLConfig.h"
#include "Entity.h"

namespace Banshee
{
	class EntityManager
	{
	public:
		BANSHEE_ENGINE static EntityManager& Instance();
		BANSHEE_ENGINE std::shared_ptr<Entity> CreateEntity();
		std::vector<std::shared_ptr<Entity>>& GetAllEntities() { return m_Entities; }

	private:
		EntityManager() {};
		~EntityManager() {};

		EntityManager(const EntityManager&) = delete;
		EntityManager(EntityManager&&) = delete;
		void operator=(const EntityManager&) = delete;
		void operator=(EntityManager&&) = delete;

	private:
		static EntityManager s_EntityManager;
		static uint32 m_TotalEntities;
		std::vector<std::shared_ptr<Entity>> m_Entities;
	};
} // End of Banshee namespace