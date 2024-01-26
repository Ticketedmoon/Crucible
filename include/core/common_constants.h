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
    static constexpr float DT = 0.01;
    static const uint32_t APP_FRAME_RATE = 60;

    static const uint32_t WINDOW_WIDTH = 1280; // 20 tiles
    static const uint32_t WINDOW_HEIGHT = 768; // 12 tiles
    static inline constexpr uint8_t TILE_SIZE = 64;

    static const inline float DEFAULT_SFX_VOLUME = 50.0f;

    constexpr float PLAYER_SIZE = 50;

    enum class EntityType
    {
        PLAYER,
        WALL,
        NONE
    };

    struct LightRayIntersect
    {
        bool hasIntersection{};
        Vec2 collisionPoint;
    };
}

#endif //CRUCIBLE_COMMON_CONSTANTS_H
