#pragma once

#ifndef CRUCIBLE_RAY_H
#define CRUCIBLE_RAY_H

#include <SFML/Graphics/Vertex.hpp>
#include "vertex.h"
#include "component.h"
#include <iostream>

namespace Crucible
{
    // TODO @investigate: Should ray contain collision information? e.g., collision point, corner points of collision side
    class Ray
    {
        public:
            Ray();
            explicit Ray(Vec2& entityPosition, Vec2 scaleFactor);
            Ray(const Ray& ray);

            void scale();

            [[nodiscard]] Vec2 getStartVertex() const;
            [[nodiscard]] Vec2 getEndVertex() const;
            [[nodiscard]] Vec2 getScaleFactor() const;

        private:
            Vec2 m_entityPosition;
            Vec2 m_scaleFactor;
            Crucible::Vertex m_endVertex;
    };
}

#endif //CRUCIBLE_RAY_H
