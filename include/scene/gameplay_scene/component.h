#pragma once

#ifndef CRUCIBLE_COMPONENT_H
#define CRUCIBLE_COMPONENT_H

#include <utility>
#include <SFML/Graphics/VertexArray.hpp>
#include "vec2.h"
#include <SFML/System/Vector3.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "common_constants.h"
#include "vertex.h"
#include "ray.h"
#include "tile.h"

struct SpriteAnimationTicker
{
    float timeBeforeAnimationUpdate;
    float animationUpdateTime;
};

namespace Component
{
    struct CTransform
    {
        std::shared_ptr<Crucible::Vec2> position;
        Crucible::Vec2 dimensions;

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

        std::shared_ptr<sf::Texture> texture;

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

    struct CAnimation
    {
        std::string animationSpriteSheetPath;

        std::vector<TileType> animationList{};

        uint8_t currentAnimationFrameIdx;

        SpriteAnimationTicker animationTicker{0, 1.0f/6.0f};

        bool has{};
    };
}

#endif //CRUCIBLE_COMPONENT_H
