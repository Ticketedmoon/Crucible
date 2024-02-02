#pragma once

#ifndef CRUCIBLE_TILE_H
#define CRUCIBLE_TILE_H

#include <SFML/System/Vector2.hpp>
#include <cstdint>

#include <SFML/Graphics/VertexArray.hpp>
#include <utility>
#include <memory>

enum class TileType
{
    TRANSPARENT = 0,
    BACKGROUND_PURPLE_WALL = 1,
    TOP_CORNER_WALL_BROKEN_PURPLE = 2,
    TOP_WALL_BROKEN_PURPLE = 3,
    BORDER_WALL_BROKEN_PURPLE = 4,
    BOTTOM_CORNER_WALL_BROKEN_PURPLE = 5,
    BOTTOM_WALL_BROKEN_PURPLE = 6,
    COLUMN_WALL_BROKEN_PURPLE = 7,
    CENTRAL_WALL_SMALL_BROKEN_PURPLE = 8,
    CENTRAL_WALL_MEDIUM_BROKEN_PURPLE = 9,
    CENTRAL_WALL_LARGE_BROKEN_PURPLE = 10
};

enum class TileRotation
{
    NONE,
    LEFT,
    RIGHT,
    TOP,
    FLIPPED_LEFT,
    FLIPPED_RIGHT
};

namespace Crucible
{
    // Flags currently used by Tiled map editor for horizontally,
    // vertically and anti-diagonally flipped tiles
    const unsigned long tiledRotationFlagsDiagonal = std::stoul("0x20000000", nullptr, 16);
    const unsigned long tiledRotationFlagsHorizontal = std::stoul("0x80000000", nullptr, 16);
    const unsigned long tiledRotationFlagsVertical = std::stoul("0x40000000", nullptr, 16);
    // Resolved flags for an easier use of the flags above
    // Left rotation is tiledRotationFlagsDiagonal + tiledRotationFlagsVertical
    static inline const unsigned long LEFT = tiledRotationFlagsDiagonal + tiledRotationFlagsVertical;
    // Right rotation is tiledRotationFlagsHorizontal + tiledRotationFlagsDiagonal
    static inline const unsigned long RIGHT = tiledRotationFlagsHorizontal + tiledRotationFlagsDiagonal;
    // Top rotation is tiledRotationFlagsHorizontal + tiledRotationFagsVertical
    static inline const unsigned long TOP = tiledRotationFlagsHorizontal + tiledRotationFlagsVertical;
    // x2 left rotation
    static inline const unsigned long FLIPPED_LEFT = LEFT * 2;
    // x2 right rotation
    static inline const unsigned long FLIPPED_RIGHT = RIGHT * 2;
}

class Tile
{
    public:
        Tile();
        Tile(sf::Vector2u position, TileType type, TileRotation tileRotation, std::shared_ptr<sf::VertexArray> vertices);

    public:
        sf::Vector2u position;
        TileType type;
        TileRotation rotation;
        std::shared_ptr<sf::VertexArray> vertices;
};

#endif //CRUCIBLE_TILE_H
