#pragma once

#ifndef CRUCIBLE_LEVEL_H
#define CRUCIBLE_LEVEL_H

#include <cstdint>
#include <vector>
#include <string>

struct TileList
{
    std::string name;
    std::vector<uint8_t> data;
};

class Level
{
    public:
        uint8_t width;
        uint8_t height;
        TileList layers;
};


#endif //CRUCIBLE_LEVEL_H
