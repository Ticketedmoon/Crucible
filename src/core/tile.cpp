#include "tile.h"

Tile::Tile() : position(), vertices()
{

}

Tile::Tile(sf::Vector2u position, std::shared_ptr<sf::VertexArray> vertices)
    : position(position), vertices(std::move(vertices))
{

}
