#pragma once

#ifndef CRUCIBLE_ENTITY_H
#define CRUCIBLE_ENTITY_H

#include <ranges>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "core/engine/component.h"
#include "entity_memory_pool.h"

class Entity
{
    public:
        friend class EntityManager;

    public:
        explicit Entity(size_t id);
        [[nodiscard]] size_t getId() const;
        void destroy();
        
    public:
        template <typename T>
        T& getComponent() const
        {
            return EntityMemoryPool::instance().getComponent<T>(m_id);
        }

        template <typename T, typename... TArgs>
        T& addComponent(TArgs&&... mArgs)
        {
            return EntityMemoryPool::instance().addComponent<T>(m_id, mArgs...);
        }

        template<typename T>
        [[nodiscard]] bool hasComponent() const
        {
            return EntityMemoryPool::instance().hasComponent<T>(m_id);
        }

    private:
        size_t m_id;

};


#endif //CRUCIBLE_ENTITY_H
