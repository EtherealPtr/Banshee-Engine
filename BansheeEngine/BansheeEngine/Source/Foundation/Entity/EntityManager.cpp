#include "EntityManager.h"

namespace Banshee
{
    uint32 EntityManager::m_TotalEntities{ 0 };
    std::vector<std::shared_ptr<Entity>> EntityManager::m_Entities{};

    std::shared_ptr<Entity> EntityManager::CreateEntity()
    {
        auto entity = std::make_shared<Entity>(++m_TotalEntities);
        m_Entities.push_back(entity);
        return entity;
    }

    const std::vector<std::shared_ptr<Entity>>& EntityManager::GetAllEntities() noexcept
    {
        return m_Entities;
    }
} // End of Banshee namespace