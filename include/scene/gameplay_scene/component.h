#pragma once

#ifndef CRUCIBLE_COMPONENT_H
#define CRUCIBLE_COMPONENT_H

#include <SFML/System/Vector2.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <utility>
#include "vec2.h"

namespace Component
{
    struct CTransform
    {
        Vec2 position;

        bool has{};
    };

    struct CControllable
    {
        bool isMovingUp;
        bool isMovingDown;
        bool isMovingLeft;
        bool isMovingRight;

        bool has;
    };

    struct CCollision
    {
        bool isCollidingUp;
        bool isCollidingDown;
        bool isCollidingLeft;
        bool isCollidingRight;

        bool has;
    };

    struct CShape
    {
        sf::VertexArray vertices = sf::VertexArray(sf::LineStrip, 5);

        bool has{};
    };
}

#endif //CRUCIBLE_COMPONENT_H
