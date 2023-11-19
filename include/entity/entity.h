#pragma once

#ifndef CRUCIBLE_ENTITY_H
#define CRUCIBLE_ENTITY_H

#include <ranges>
#include <cstddef>
#include <memory>
#include <unordered_map>
#include <vector>
#include <algorithm>

#include "component.h"

class Entity
{
    public:
        friend class EntityManager;

        template <typename T>
        [[nodiscard]] bool hasComponent() const;

        template <typename T>
        [[nodiscard]] bool hasComponents(std::vector<T> components) const;

        template <typename T, typename... TArgs>
        void addComponent(TArgs&&... mArgs);

        template <typename T>
        T& getComponent();

        template <typename T>
        void removeComponent();


    public:
        enum class Type
        {
            PLAYER,
            NONE
        };

        [[nodiscard]] size_t getId() const;
        Type getType();
        [[nodiscard]] bool isAlive() const;
        void destroy();

    private:
        Entity(size_t id, Type type);

    private:
        size_t m_id;
        Type m_type;
        bool m_isAlive;

        std::tuple<Component::CTransform> m_components;

};


#endif //CRUCIBLE_ENTITY_H
