#pragma once

#ifndef CRUCIBLE_ENTITY_MANAGER_H
#define CRUCIBLE_ENTITY_MANAGER_H


#include <cstddef>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>
#include "entity.h"

class EntityManager
{
    public:
        void update();
        Entity addEntity(Crucible::EntityType type);
        std::vector<Entity> getEntities();
        std::vector<Entity>& getEntitiesByEntityType(Crucible::EntityType entityType);

        template<typename T>
        std::vector<Entity> getEntitiesByComponentType()
        {
            std::ranges::filter_view filteredEntities = m_entities |
                    std::ranges::views::filter([](Entity& e) {
                        return e.hasComponent<T>();
                    });
            return std::vector(filteredEntities.begin(), filteredEntities.end());
        };

        // TODO is this needed?
        template<typename T>
        std::vector<T> getActiveComponentGroupForEntities()
        {
            std::vector<T> activeComponents;
            std::vector<T> componentGroup = EntityMemoryPool::instance().getComponents<T>();
            for (T component : componentGroup)
            {
                if (component.has)
                {
                    activeComponents.emplace_back(component);
                }
            }
            return activeComponents;
        }

    private:
        std::vector<Entity> m_entities;
        std::vector<std::pair<Crucible::EntityType, Entity>> m_entitiesToAdd;
        std::unordered_map<Crucible::EntityType, std::vector<Entity>> m_entitiesByType;

};


#endif //CRUCIBLE_ENTITY_MANAGER_H
