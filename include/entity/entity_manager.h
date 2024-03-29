#pragma once

#ifndef CRUCIBLE_ENTITY_MANAGER_H
#define CRUCIBLE_ENTITY_MANAGER_H

#include <cstddef>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>
#include "entity.h"

static const uint8_t TOTAL_ENTITY_TYPES = 100;

class EntityManager
{
    public:
        EntityManager();

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

        template<typename T>
        std::vector<T> getComponentsByEntityIds()
        {
            std::vector<T> components;
            std::ranges::filter_view filteredEntities = m_entities |
                    std::ranges::views::filter([components](Entity& e) {
                        if (e.hasComponent<T>())
                        {
                            components.emplace_back(e.getComponent<T>());
                        }
                    });
            return components;
        };

    private:
        void removeDeadEntities(std::vector<Entity>& entities);

    private:
        std::vector<Entity> m_entities;
        std::vector<std::pair<Crucible::EntityType, Entity>> m_entitiesToAdd;
        std::unordered_map<Crucible::EntityType, std::vector<Entity>> m_entitiesByType;

};


#endif //CRUCIBLE_ENTITY_MANAGER_H
