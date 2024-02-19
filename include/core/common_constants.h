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

    static const uint32_t WINDOW_WIDTH = 1280; // 80 tiles
    static const uint32_t WINDOW_HEIGHT = 768; // 48 tiles
    static inline constexpr uint8_t TILE_SIZE = 16;

    static const inline float DEFAULT_SFX_VOLUME = 50.0f;

    static const inline std::vector<Crucible::Vec2> PLAYER_SPAWN_LOCATIONS{
            {2 * Crucible::TILE_SIZE, 3 * Crucible::TILE_SIZE},
            {2 * Crucible::TILE_SIZE, 8 * Crucible::TILE_SIZE}
    };

    static const inline uint8_t TOTAL_ENTITY_TYPES = 4;

    enum class EntityType
    {
        PLAYER,
        GUARD,
        LEVEL_OBJECT,
        NONE
    };

    struct LightRayIntersect
    {
        bool hasIntersection{};
        EntityType entityType{};
        Vec2 collisionPoint;
    };
}

#endif //CRUCIBLE_COMMON_CONSTANTS_H
