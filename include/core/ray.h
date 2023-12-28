#pragma once

#ifndef CRUCIBLE_RAY_H
#define CRUCIBLE_RAY_H

#include <SFML/Graphics/Vertex.hpp>
#include "vertex.h"
#include <iostream>
#include <memory>

// TODO @investigate: Should ray contain collision information? e.g., collision point, corner points of collision side
namespace Crucible
{
    class Ray
    {
        public:
            Ray();
            ~Ray();
            explicit Ray(std::shared_ptr<Vec2> entityPosition, Vec2 scaleFactor);

            // assignment operator overload.
            Ray& operator=(const Ray& other);

            void scale();

            [[nodiscard]] std::shared_ptr<Vec2>& getStartVertex();
            [[nodiscard]] Vec2 getEndVertex() const;
            [[nodiscard]] Vec2 getScaleFactor() const;

        private:
            std::shared_ptr<Vec2> m_entityPosition;
            Vec2 m_scaleFactor;
            Crucible::Vertex m_endVertex;
    };
}

#endif //CRUCIBLE_RAY_H
