#pragma once

#ifndef CRUCIBLE_COMPONENT_H
#define CRUCIBLE_COMPONENT_H

#include <SFML/System/Vector2.hpp>
#include <utility>
#include <SFML/Graphics/VertexArray.hpp>
#include "vec2.h"
#include "common_constants.h"

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
        sf::VertexArray vertices = sf::VertexArray(sf::LineStrip);

        bool has{};
    };

    struct CLightSource
    {
        // Every 2 elements is a line, so to access the nth line, the index positions are: (v[i * 2], v[(i * 2) * 1])
        sf::VertexArray rayVertices;
        sf::VertexArray lightVertices;

        std::vector<std::vector<Crucible::LightRayIntersect>> lightRayIntersects;

        bool has{};
    };
}

#endif //CRUCIBLE_COMPONENT_H
