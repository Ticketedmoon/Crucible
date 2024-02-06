#pragma once

#ifndef CRUCIBLE_COMPONENT_H
#define CRUCIBLE_COMPONENT_H

#include <utility>
#include <SFML/Graphics/VertexArray.hpp>
#include "vec2.h"
#include <SFML/System/Vector3.hpp>
#include "common_constants.h"
#include "vertex.h"
#include "ray.h"
#include "tile.h"

namespace Component
{
    struct CTransform
    {
        std::shared_ptr<Crucible::Vec2> position;

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

    struct CCollider
    {
        bool immovable{};

        bool has{};
    };

    struct CTile
    {
        Tile tile;

        bool has{};
    };

    struct CPathFollower
    {
        std::vector<Crucible::Vec2> path;

        std::string pathingObjectLayerName;

        size_t destinationIndex{0};

        bool has{};
    };

    struct CLightSource
    {
        // Every 2 elements is a line, so to access the nth line, the index positions are: (v[i * 2], v[(i * 2) * 1])
        std::vector<Crucible::Ray> rays;

        sf::VertexArray lightVertices;

        std::vector<std::vector<Crucible::LightRayIntersect>> lightRayIntersects;

        std::string lightingObjectLayerName;

        bool has{};
    };
}

#endif //CRUCIBLE_COMPONENT_H
