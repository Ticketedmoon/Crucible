#include "ray.h"

namespace Crucible
{
    Ray::Ray() : m_entityPosition({}), m_scaleFactor(Vec2())
    {

    }
    Ray::~Ray() = default;

    Ray::Ray(Vec2 entityPosition, Vec2 scaleFactor) : m_entityPosition(entityPosition),
                                                      m_scaleFactor(scaleFactor)
    {

    }

    // assignment operator function
    Ray& Ray::operator=(const Ray& other)
    {
        std::cout << "[Ray] Assignment Operator Override called." << '\n';
        m_entityPosition = other.m_entityPosition;
        m_scaleFactor = other.m_scaleFactor;
        m_endVertex = other.m_endVertex;
        return *this;
    }

    void Ray::scale()
    {
        // Scale
        m_endVertex.position += m_scaleFactor;
    }

    Vec2& Ray::getStartVertex()
    {
        return m_entityPosition;
    }

    Vec2 Ray::getEndVertex() const
    {
        return m_endVertex.position;
    }

    Vec2 Ray::getScaleFactor() const
    {
        return m_scaleFactor;
    }
}