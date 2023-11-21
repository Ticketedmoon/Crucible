#include "entity/entity.hpp"

Entity::Entity(size_t id, Entity::Type type) : m_id(id), m_type(type), m_isAlive(true)
{
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
