#pragma once

#ifndef CRUCIBLE_TILE_H
#define CRUCIBLE_TILE_H

#include <SFML/System/Vector2.hpp>
#include <utility>
#include <cstdint>

#include <SFML/Graphics/VertexArray.hpp>
#include <utility>
#include <memory>

enum class EntityAnimation
{
    PLAYER_WALK_DOWN,
    PLAYER_WALK_UP,
    PLAYER_WALK_RIGHT,
    PLAYER_WALK_LEFT,

    PLAYER_IDLE_DOWN,
    PLAYER_IDLE_UP,
    PLAYER_IDLE_RIGHT,
    PLAYER_IDLE_LEFT,

    SHADOW_MAGIC_CAST
};

enum class EntityDirection
{
        DOWN,
        UP,
        RIGHT,
        LEFT
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
        Tile(sf::Vector2u position, std::shared_ptr<sf::VertexArray> vertices);

    public:
        sf::Vector2u position;
        std::shared_ptr<sf::VertexArray> vertices;
};

#endif //CRUCIBLE_TILE_H
