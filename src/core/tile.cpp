#include "tile.h"

#include <utility>

Tile::Tile() : position(), type(), vertices()
{

}

Tile::Tile(sf::Vector2u position, TileType type, std::shared_ptr<sf::VertexArray> vertices)
    : position(position), type(type), vertices(std::move(vertices))
{

}
