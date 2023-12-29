#include "ray.h"

#include <utility>

namespace Crucible
{
    Ray::Ray() : m_entityPosition(), m_endVertex(Vec2())
    {

    }
    Ray::~Ray() = default;

    Ray::Ray(std::shared_ptr<Vec2> entityPosition, Vec2 endPosition) : m_entityPosition(std::move(entityPosition)),
                                                                       m_endVertex(endPosition)
    {

    }

    // assignment operator function
    Ray& Ray::operator=(const Ray& other)
    {
        if (this != &other)
        {
            m_entityPosition = other.m_entityPosition;
            m_endVertex = other.m_endVertex;
        }

        return *this;
    }

    void Ray::scale()
    {
        //
        // Scale
        m_endVertex.position += m_scaleFactor;
    }

    std::shared_ptr<Vec2>& Ray::getStartVertex()
    {
        return m_entityPosition;
    }

    Vec2 Ray::getEndVertex() const
    {
        return m_endVertex.position;
    }
}