#include "tile.h"

#include <utility>

Tile::Tile() : position(), type(), vertices()
{

}

Tile::Tile(sf::Vector2u position, TileType type, TileRotation tileRotation, std::shared_ptr<sf::VertexArray> vertices)
    : position(position), type(type), rotation(tileRotation), vertices(std::move(vertices))
{

}
