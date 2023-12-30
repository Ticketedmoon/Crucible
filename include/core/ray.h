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
            explicit Ray(std::shared_ptr<Vec2> entityPosition, Vec2 endVertex);

            // assignment operator overload.
            Ray& operator=(const Ray& other);

            [[nodiscard]] std::shared_ptr<Vec2>& getStartVertex();
            [[nodiscard]] Vec2 getEndVertex() const;

        private:
            std::shared_ptr<Vec2> m_entityPosition;
            Crucible::Vertex m_endVertex;
    };
}

#endif //CRUCIBLE_RAY_H
