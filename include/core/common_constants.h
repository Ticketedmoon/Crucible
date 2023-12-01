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
    static constexpr uint32_t MAX_LEVEL_WIDTH = 16384;
    static constexpr uint32_t LEVEL_DEPTH_KILL_ZONE = 768;
    static constexpr uint32_t MAX_LEVEL_HEIGHT = 2048;
    static inline constexpr uint8_t TILE_SIZE = 32;

    static const inline float DEFAULT_SFX_VOLUME = 50.0f;
    static const inline float JUMP_SFX_VOLUME = 5.0f;

    enum class EntityType
    {
        PLAYER,
        WALL,
        NONE
    };

    struct LightRayIntersect
    {
        bool result{};
        Vec2 pos;
    };
}

#endif //CRUCIBLE_COMMON_CONSTANTS_H
