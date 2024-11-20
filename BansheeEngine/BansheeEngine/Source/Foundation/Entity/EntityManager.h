#pragma once

#include "Entity.h"

namespace Banshee
{
	class EntityManager
	{
    public:
        BANSHEE_ENGINE static const std::vector<const Entity*>& GetAllEntities() noexcept { return s_Entities; }
        static uint32 RegisterEntity(const Entity* _entity);

    private:
        static uint32 s_TotalEntities;
        static std::vector<const Entity*> s_Entities;
	};
} // End of Banshee namespace