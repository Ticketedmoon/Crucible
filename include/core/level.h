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

struct Layer
{
    std::string name;
    std::string type;
};

struct TileLayer : Layer
{
    std::vector<Tile> data;
};

struct ObjectLayer : Layer
{
    std::vector<std::shared_ptr<sf::VertexArray>> tileObjectVertices;
};

class Level
{
    public:
        uint8_t width;
        uint8_t height;
        std::vector<TileLayer> tileLayers;
        std::vector<ObjectLayer> objectLayers;
};


#endif //CRUCIBLE_LEVEL_H
