#include "tile.h"

#include <utility>

Tile::Tile() : position(), tileIdx(), rotation(TileRotation::NONE), vertices()
{

}

Tile::Tile(sf::Vector2u position, size_t tileIdx, TileRotation tileRotation, std::shared_ptr<sf::VertexArray> vertices)
    : position(position), tileIdx(tileIdx), rotation(tileRotation), vertices(std::move(vertices))
{

}
