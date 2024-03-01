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

struct GameTimeTicker
{
    double currentTime{0.0f};
    double timeUntilUpdate;
};

struct Waypoint
{
    Crucible::Vec2 position;
    uint32_t waitPeriodMs{0};
};

namespace Component
{
    struct CTransform
    {
        // TODO investigate if this needs to be a shared_ptr
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
        std::vector<Waypoint> path;

        std::string pathingObjectLayerName;

        size_t destinationIndex{0};

        GameTimeTicker gameTimeTicker{0, 0};

        bool has{};
    };

    struct CLightSource
    {
        std::unordered_map<Crucible::RayType, Crucible::LightRayGroup> lightRayGroups;

        sf::VertexArray lightVertices;

        std::string lightingObjectLayerName;

        bool has{};
    };

    // TODO MOVE ME
    struct AnimationGroup {
        std::vector<uint32_t> animationIndexes;
        GameTimeTicker animationTicker{0, 1.0f/6.0f};
    };

    struct CAnimation
    {
        std::string animationSpriteSheetPath;

        EntityDirection entityDirection;
        EntityAnimation currentAnimation;

        uint32_t currentAnimationFrameIdx;
        std::unordered_map<EntityAnimation, AnimationGroup> animations;
        uint32_t spriteSheetWidth{6};

        bool has{};
    };
}

#endif //CRUCIBLE_COMPONENT_H
