#include "tile.h"

Tile::Tile() : position(), rotation(TileRotation::NONE), vertices()
{

}

Tile::Tile(sf::Vector2u position, TileRotation tileRotation, std::shared_ptr<sf::VertexArray> vertices)
    : position(position), rotation(tileRotation), vertices(std::move(vertices))
{

}
