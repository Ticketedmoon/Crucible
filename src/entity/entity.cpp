#include "entity/entity.h"

Entity::Entity(size_t id, Entity::Type type) : m_id(id), m_type(type), m_isAlive(true)
{
}

template <typename T, typename... TArgs>
void Entity::addComponent(TArgs&&... mArgs)
{
    T& component = getComponent<T>();
    component = T(std::forward<TArgs>(mArgs)...);
    component.has = true;
    return component;
}

template <typename T>
bool Entity::hasComponent() const
{
    return getComponent<T>().has();
}

template<typename T>
bool Entity::hasComponents(std::vector<T> components) const
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
T& Entity::getComponent()
{
    return std::get<T>(m_components);
}

template <typename T>
void Entity::removeComponent()
{
    T& component = getComponent<T>();
    component.has = false;
    std::remove<T>(m_components);
}

size_t Entity::getId() const
{
    return m_id;
}

Entity::Type Entity::getType()
{
    return m_type;
}

bool Entity::isAlive() const
{
    return m_isAlive;
}

void Entity::destroy()
{
    m_isAlive = false;
}
