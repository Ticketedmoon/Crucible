#include "entity/entity_manager.h"

void EntityManager::update()
{
    for (const auto& entityTypeToEntityPair : m_entitiesToAdd)
    {
        m_entities.emplace_back(entityTypeToEntityPair.second);

        if (m_entitiesByType.contains(entityTypeToEntityPair.first))
        {
            m_entitiesByType.at(entityTypeToEntityPair.first).emplace_back(entityTypeToEntityPair.second);
        }
        else
        {
            m_entitiesByType.insert({entityTypeToEntityPair.first, {entityTypeToEntityPair.second}});
        }
    }

    m_entitiesToAdd.clear();
}

Entity EntityManager::addEntity(Crucible::EntityType type)
{
    Entity e = EntityMemoryPool::instance().addEntity(type);
    m_entitiesToAdd.emplace_back(type, e);
    return e;
}

std::vector<Entity> EntityManager::getEntities()
{
    return m_entities;
}

std::vector<Entity>& EntityManager::getEntitiesByEntityType(Crucible::EntityType entityType)
{
    return m_entitiesByType.at(entityType);
}
