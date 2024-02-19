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
#include "common_constants.h"

struct CustomProperty
{
    std::string name;
    std::string type;
    std::string value;
};

struct Layer
{
    std::string name;
    std::string type;
};

struct Object
{
    Crucible::EntityType entityType;
    size_t tileIdx;
    std::shared_ptr<sf::VertexArray> objectVertices;
};

struct TileLayer : Layer
{
    std::unordered_map<std::string, sf::VertexArray> tilesetPathToLevelData;
};

struct ObjectLayer : Layer
{
    std::vector<Object> lightingObjectData;
    std::unordered_map<std::string, std::vector<CustomProperty>> customProperties;
};

class Level
{
    public:
        uint8_t width;
        uint8_t height;
        std::vector<TileLayer> tileLayers;
        std::unordered_map<std::string, ObjectLayer> layerNameToObjectLayer;
};


#endif //CRUCIBLE_LEVEL_H
