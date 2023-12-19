#include "ray.h"

Crucible::Ray::Ray() = default;

Crucible::Ray::Ray(Vec2& entityPosition, Vec2 scaleFactor) : m_entityPosition(entityPosition), m_scaleFactor(scaleFactor)
{

}

Crucible::Ray::Ray(const Crucible::Ray& ray) : m_entityPosition(ray.m_entityPosition), m_scaleFactor(ray.m_scaleFactor)
{
    std::cout << "[Ray] Copy Constructor called." << '\n';
}

void Crucible::Ray::scale()
{
    m_endVertex.position += m_scaleFactor;
}

Crucible::Vec2 Crucible::Ray::getStartVertex() const
{
    return m_entityPosition;
}

Crucible::Vec2 Crucible::Ray::getEndVertex() const
{
    return m_endVertex.position;
}

Crucible::Vec2 Crucible::Ray::getScaleFactor() const
{
    return m_scaleFactor;
}
