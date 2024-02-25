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

enum class ObjectType
{
        POLYGON = 0,
        POLYLINE = 1,
        RECT = 2
};

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

struct TileSet
{
    const size_t firstgid;
    const std::string name;
    const std::string path;
    const size_t tileCount;
    const size_t tileWidth;
    const size_t tileHeight;
    const size_t columns;
};

class Level
{
    public:
        uint8_t width;
        uint8_t height;
        std::vector<TileLayer> tileLayers;
        std::unordered_map<std::string, ObjectLayer> layerNameToObjectLayer;
        std::vector<TileSet> tileSets;
};


#endif //CRUCIBLE_LEVEL_H
