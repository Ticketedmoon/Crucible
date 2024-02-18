#pragma once

#ifndef CRUCIBLE_TILE_H
#define CRUCIBLE_TILE_H

#include <SFML/System/Vector2.hpp>
#include <cstdint>

#include <SFML/Graphics/VertexArray.hpp>
#include <utility>
#include <memory>

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
        Tile(sf::Vector2u position, size_t tileIdx, TileRotation tileRotation,
                std::shared_ptr<sf::VertexArray> vertices);

    public:
        sf::Vector2u position;
        size_t tileIdx;
        TileRotation rotation;
        std::shared_ptr<sf::VertexArray> vertices;
};

#endif //CRUCIBLE_TILE_H
