#pragma once

#ifndef CRUCIBLE_COMMON_CONSTANTS_H
#define CRUCIBLE_COMMON_CONSTANTS_H

#include <cstdint>
#include <string_view>
#include "vec2.h"
#include "ray.h"

namespace Crucible
{
    static constexpr std::string_view WINDOW_TITLE = "crucible";
    static constexpr float DT = 0.02;
    static const uint32_t APP_FRAME_RATE = 60;

    static const uint32_t WINDOW_WIDTH = 1280; // 80 tiles
    static const uint32_t WINDOW_HEIGHT = 768; // 48 tiles

    static inline constexpr uint8_t TILE_SIZE = 16;
    static const bool USE_VERTICAL_SYNC = true;
    static const bool SHOULD_CULL_TILES = false;

    static const inline float PLAYER_ZOOM_FACTOR = 0.5;
    static const inline float DEFAULT_SFX_VOLUME = 50.0f;

    static const inline std::vector<Crucible::Vec2> PLAYER_SPAWN_LOCATIONS{
            {32.5 * Crucible::TILE_SIZE, 42.5 * Crucible::TILE_SIZE},
            {48.5 * Crucible::TILE_SIZE, 42.5 * Crucible::TILE_SIZE}
    };

    static const inline uint8_t TOTAL_ENTITY_TYPES = 4;

    enum class EntityType
    {
        PLAYER,
        GUARD,
        PROJECTILE,
        LEVEL_OBJECT,
        NONE
    };

    enum class RayType
    {
            CORE,
            ADDITIONAL
    };

    struct LightRayIntersect
    {
        bool hasIntersection{};
        EntityType entityType{};
        Vec2 collisionPoint;
    };

    struct LightRayGroup
    {
        std::vector<Crucible::Ray> rays;
        std::vector<std::vector<Crucible::LightRayIntersect>> rayIntersects;
    };
}

#endif //CRUCIBLE_COMMON_CONSTANTS_H
