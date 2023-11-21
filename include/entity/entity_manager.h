#pragma once

#ifndef CRUCIBLE_ENTITY_MANAGER_H
#define CRUCIBLE_ENTITY_MANAGER_H


#include <cstddef>
#include <vector>
#include <unordered_map>
#include <memory>
#include <cassert>
#include "entity.hpp"

class EntityManager
{
    public:
        void update();
        std::shared_ptr<Entity>& addEntity(Entity::Type type);
        std::vector<std::shared_ptr<Entity>> getEntities();

        template <typename T>
        std::vector<std::shared_ptr<Entity>> getEntitiesByComponentType()
        {
            std::ranges::filter_view filteredEntities = m_entities | std::ranges::views::filter([](std::shared_ptr<Entity>& e) {
                return e->hasComponent<T>();
            });
            return std::vector(filteredEntities.begin(), filteredEntities.end());
        };

        bool hasEntityWithType(Entity::Type type);

    private:
        static void removeDeadEntities(std::vector<std::shared_ptr<Entity>>& entities);

    private:
        size_t m_totalEntities = 0;
        std::vector<std::shared_ptr<Entity>> m_entities;
        std::vector<std::shared_ptr<Entity>> m_entitiesToAdd; // Holds enemies to add each frame, will be added to m_entities on next frame update
        std::unordered_map<Entity::Type, std::vector<std::shared_ptr<Entity>>> m_entitiesByType;

};


#endif //CRUCIBLE_ENTITY_MANAGER_H
