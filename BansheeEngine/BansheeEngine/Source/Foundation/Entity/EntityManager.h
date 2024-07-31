#pragma once

#include "Entity.h"

namespace Banshee
{
	class EntityManager
	{
    public:
        BANSHEE_ENGINE static std::shared_ptr<Entity> CreateEntity();
        BANSHEE_ENGINE static const std::vector<std::shared_ptr<Entity>>& GetAllEntities() noexcept;

    private:
        static uint32 m_TotalEntities;
        static std::vector<std::shared_ptr<Entity>> m_Entities;
	};
} // End of Banshee namespace