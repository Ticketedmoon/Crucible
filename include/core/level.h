#pragma once

#ifndef CRUCIBLE_LEVEL_H
#define CRUCIBLE_LEVEL_H

#include <cstdint>
#include <vector>
#include <string>
#include <SFML/Graphics/Rect.hpp>
#include <memory>
#include "tile.h"
#include "vec2.h"

struct TileLayer
{
    std::string name;
    std::vector<Tile> data;
    std::vector<std::shared_ptr<sf::VertexArray>> tileObjectsVertexLayer;
};

class Level
{
    public:
        uint8_t width;
        uint8_t height;
        std::vector<TileLayer> layers;
};


#endif //CRUCIBLE_LEVEL_H
