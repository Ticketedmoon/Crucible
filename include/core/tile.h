#pragma once

#ifndef CRUCIBLE_TILE_H
#define CRUCIBLE_TILE_H

#include <SFML/System/Vector2.hpp>
#include <utility>
#include <cstdint>

#include <SFML/Graphics/VertexArray.hpp>
#include <utility>
#include <memory>

enum class PlayerAnimation
{
    PLAYER_WALK_DOWN_START,
    PLAYER_WALK_UP_START,
    PLAYER_WALK_RIGHT_START,
    PLAYER_WALK_LEFT_START,
    PLAYER_IDLE_START
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
