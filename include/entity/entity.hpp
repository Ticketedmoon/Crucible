#pragma once

#ifndef CRUCIBLE_ENTITY_HPP
#define CRUCIBLE_ENTITY_HPP

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

    public:
        enum class Type
        {
            PLAYER,
            WALL,
            NONE
        };

        [[nodiscard]] size_t getId() const;
        Type getType();
        [[nodiscard]] bool isAlive() const;
        void destroy();
        
    public:
        template <typename T>
        T& getComponent()
        {
            return std::get<T>(m_components);
        }

        template <typename T, typename... TArgs>
        T& addComponent(TArgs&&... mArgs)
        {
            T& component = getComponent<T>();
            component = T(std::forward<TArgs>(mArgs)...);
            component.has = true;
            return component;
        }

        template<typename T>
        [[nodiscard]] bool hasComponent()
        {
            return getComponent<T>().has;
        }

        template<typename T>
        bool hasComponents(std::vector<T> components) const
        {
            for (T component: components)
            {
                if (!hasComponent<T>(component))
                {
                    return false;
                }
            }
            return true;
        }

        template <typename T>
        void removeComponent()
        {
            T& component = getComponent<T>();
            component.has = false;
            std::remove<T>(m_components);
        }

    private:
        Entity(size_t id, Type type);

    private:
        size_t m_id;
        Type m_type;
        bool m_isAlive;

        std::tuple<Component::CTransform, Component::CControllable, Component::CRectangleShape> m_components;

};


#endif //CRUCIBLE_ENTITY_HPP
