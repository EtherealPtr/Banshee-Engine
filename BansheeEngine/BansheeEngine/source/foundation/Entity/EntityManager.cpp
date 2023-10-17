#include "EntityManager.h"

namespace Banshee
{
    uint32 EntityManager::m_TotalEntities = 0;

    EntityManager& EntityManager::Instance()
    {
        static EntityManager s_EntityManager;
        return s_EntityManager;
    }

    std::shared_ptr<Entity> EntityManager::CreateEntity()
    {
        std::shared_ptr<Entity> entity = std::make_shared<Entity>(++m_TotalEntities);
        m_Entities.emplace_back(entity);
        return entity;
    }
} // End of Banshee namespace