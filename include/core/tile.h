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
    NONE = -1,
    TRANSPARENT = 0,

    // player tileset

    // down
    PLAYER_WALK_DOWN_A = 1,
    PLAYER_WALK_DOWN_B = 2,
    PLAYER_WALK_DOWN_C = 3,
    PLAYER_WALK_DOWN_D = 4,
    PLAYER_WALK_DOWN_E = 5,
    PLAYER_WALK_DOWN_F = 6,

    // up
    PLAYER_WALK_UP_A = 7,
    PLAYER_WALK_UP_B = 8,
    PLAYER_WALK_UP_C = 9,
    PLAYER_WALK_UP_D = 10,
    PLAYER_WALK_UP_E = 11,
    PLAYER_WALK_UP_F = 12,

    // right
    PLAYER_WALK_RIGHT_A = 13,
    PLAYER_WALK_RIGHT_B = 14,
    PLAYER_WALK_RIGHT_C = 15,
    PLAYER_WALK_RIGHT_D = 16,
    PLAYER_WALK_RIGHT_E = 17,
    PLAYER_WALK_RIGHT_F = 18,

    // left
    PLAYER_WALK_LEFT_A = 19,
    PLAYER_WALK_LEFT_B = 20,
    PLAYER_WALK_LEFT_C = 21,
    PLAYER_WALK_LEFT_D = 22,
    PLAYER_WALK_LEFT_E = 23,
    PLAYER_WALK_LEFT_F = 24,

    // basic tileset
    SPAWN_ZONE = 23,
    END_ZONE = 24,

    // dungeon tileset
    BACKGROUND_PURPLE_WALL = 10,
    TOP_CORNER_WALL_BROKEN_PURPLE = 1,
    TOP_WALL_BROKEN_PURPLE = 2,
    BORDER_WALL_BROKEN_PURPLE = 9,
    BOTTOM_CORNER_WALL_BROKEN_PURPLE = 17,
    BOTTOM_WALL_BROKEN_PURPLE = 18,
    COLUMN_WALL_BROKEN_PURPLE = 36,
    CENTRAL_WALL_SMALL_BROKEN_PURPLE = 25,
    CENTRAL_WALL_MEDIUM_BROKEN_PURPLE = 11,
    CENTRAL_WALL_LARGE_BROKEN_PURPLE = 37,
};

enum class TileRotation
{
    NONE,
    ROTATED_LEFT,
    ROTATED_RIGHT,
    FLIPPED_HORIZONTALLY,
    FLIPPED_VERTICALLY
};

namespace Crucible
{
    // Bits on the far end of the 32-bit global tile ID are used for tile flags
    const unsigned FLIPPED_HORIZONTALLY_FLAG = 0x80000000;
    const unsigned FLIPPED_VERTICALLY_FLAG = 0x40000000;
    const unsigned FLIPPED_DIAGONALLY_FLAG = 0x20000000;
    const unsigned ROTATED_HEXAGONAL_120_FLAG = 0x10000000;
}

class Tile
{
    public:
        Tile();
        Tile(sf::Vector2u position, TileType type, TileRotation tileRotation,
                std::shared_ptr<sf::VertexArray> vertices);

    public:
        sf::Vector2u position;
        TileType type;
        TileRotation rotation;
        std::shared_ptr<sf::VertexArray> vertices;
};

#endif //CRUCIBLE_TILE_H
