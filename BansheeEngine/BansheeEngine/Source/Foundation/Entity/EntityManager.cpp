#include "EntityManager.h"

namespace Banshee
{
    uint32 EntityManager::s_TotalEntities{ 0 };
    std::vector<const Entity*> EntityManager::s_Entities{};

    EntityManager::EntityManager() noexcept
    {
        s_Entities.reserve(50);
    }

    uint32 EntityManager::RegisterEntity(const Entity* _entity)
    {
        s_Entities.emplace_back(_entity);
        return static_cast<uint32>(s_Entities.size());
    }
} // End of Banshee namespace