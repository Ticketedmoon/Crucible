#pragma once

#ifndef CRUCIBLE_TILE_H
#define CRUCIBLE_TILE_H

#include <SFML/System/Vector2.hpp>
#include <utility>
#include <cstdint>

#include <SFML/Graphics/VertexArray.hpp>
#include <utility>
#include <memory>

// Player Anim
enum class PlayerAnimation
{
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

    const unsigned FLIPPED_LEFT_FLAG = Crucible::FLIPPED_DIAGONALLY_FLAG + Crucible::FLIPPED_VERTICALLY_FLAG;
    const unsigned FLIPPED_RIGHT_FLAG = Crucible::FLIPPED_HORIZONTALLY_FLAG + Crucible::FLIPPED_DIAGONALLY_FLAG;
    const unsigned FLIPPED_TOP_FLAG = Crucible::FLIPPED_HORIZONTALLY_FLAG + Crucible::FLIPPED_VERTICALLY_FLAG;
}

class Tile
{
    public:
        Tile();
        Tile(sf::Vector2u position, TileRotation tileRotation, std::shared_ptr<sf::VertexArray> vertices);

    public:
        sf::Vector2u position;
        TileRotation rotation;
        std::shared_ptr<sf::VertexArray> vertices;
};

#endif //CRUCIBLE_TILE_H
