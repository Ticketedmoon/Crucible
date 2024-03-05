#include "entity/entity_manager.h"

EntityManager::EntityManager()
{
    // Initialise entityType->Entity map with empty vectors.
    for (size_t i = 0; i < TOTAL_ENTITY_TYPES; i++)
    {
        auto entityType = static_cast<Crucible::EntityType>(i);
        if (i < Crucible::TOTAL_ENTITY_TYPES)
        {
            m_entitiesByType[entityType] = {};
        }
    }
}

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

    removeDeadEntities(m_entities);

    for (auto& item: m_entitiesByType)
    {
        removeDeadEntities(item.second);
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

void EntityManager::removeDeadEntities(std::vector<Entity>& entities)
{
    const auto& callback = std::remove_if(
            entities.begin(),
            entities.end(),
            [](Entity& entity) {
                return !EntityMemoryPool::instance().isEntityAlive(entity.getId());
            });
    entities.erase(callback, entities.end());
}

