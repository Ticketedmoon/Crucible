#pragma once

#ifndef CRUCIBLE_COMMON_CONSTANTS_H
#define CRUCIBLE_COMMON_CONSTANTS_H

#include <cstdint>
#include <string_view>
#include "vec2.h"

namespace Crucible
{
    static constexpr std::string_view WINDOW_TITLE = "crucible";
    static const bool USE_VERTICAL_SYNC = true;
    static const uint32_t APP_FRAME_RATE = 60;
    static const uint32_t WINDOW_WIDTH = 1280; // 20 tiles
    static const uint32_t WINDOW_HEIGHT = 768; // 12 tiles
    static constexpr float DT = 0.01;
    static inline constexpr uint8_t TILE_SIZE = 32;

    static const inline float DEFAULT_SFX_VOLUME = 50.0f;

    constexpr float PLAYER_SIZE = 50;

    constexpr uint8_t TOTAL_CORNERS_FOR_RAY = 2;
    constexpr uint8_t TOTAL_RAYS_PER_CORNER = 4;
    constexpr uint8_t SIDES_PER_SHAPE_VISIBLE = 2;
    constexpr uint8_t SHAPES_VISIBLE = 4;
    constexpr size_t TOTAL_ADDITIONAL_CORNER_RAYS = TOTAL_RAYS_PER_CORNER * TOTAL_CORNERS_FOR_RAY * SIDES_PER_SHAPE_VISIBLE * SHAPES_VISIBLE;

    constexpr size_t TOTAL_CORE_LIGHT_RAYS = 3;
    constexpr size_t TOTAL_RAYS = TOTAL_CORE_LIGHT_RAYS + TOTAL_ADDITIONAL_CORNER_RAYS;

    constexpr int TOTAL_DEGREES_CIRCLE = 360;
    constexpr int DEGREE_INCREMENT = TOTAL_DEGREES_CIRCLE / Crucible::TOTAL_CORE_LIGHT_RAYS;
    constexpr int RAY_SPEED = 10000;

    enum class EntityType
    {
        PLAYER,
        WALL,
        NONE
    };

    struct LightRayIntersect
    {
        bool hasIntersection{};

        bool isShapeIntersection{};

        Vec2 collisionPoint;

        Vec2 collidedLineStartVertex;
        Vec2 collidedLineEndVertex;
    };
}

#endif //CRUCIBLE_COMMON_CONSTANTS_H
