#include "EntityManager.h"

namespace Banshee
{
    uint32 EntityManager::s_TotalEntities{ 0 };
    std::vector<std::shared_ptr<Entity>> EntityManager::s_Entities{};

    std::shared_ptr<Entity> EntityManager::CreateEntity()
    {
        auto entity = std::make_shared<Entity>(++s_TotalEntities);
        s_Entities.push_back(entity);
        return entity;
    }
} // End of Banshee namespace