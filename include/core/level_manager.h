#pragma once

#ifndef CRUCIBLE_LEVEL_MANAGER_H
#define CRUCIBLE_LEVEL_MANAGER_H

#include <json.hpp>
#include <fstream>
#include <iostream>

#include <SFML/Graphics/Texture.hpp>

#include "tile.h"
#include "level.h"
#include "texture_manager.h"
#include "common_constants.h"
#include "entity_manager.h"

// Flags currently used by Tiled map editor for horizontally,
// vertically and anti-diagonally flipped tiles
const unsigned long tiledRotationFlagsDiagonal = std::stoul("0x20000000", nullptr, 16);
const unsigned long tiledRotationFlagsHorizontal = std::stoul("0x80000000", nullptr, 16);
const unsigned long tiledRotationFlagsVertical = std::stoul("0x40000000", nullptr, 16);

// Resolved flags for an easier use of the flags above
struct TiledRotationOffset
{
    // Left rotation is tiledRotationFlagsDiagonal + tiledRotationFlagsVertical
    const unsigned long left = tiledRotationFlagsDiagonal + tiledRotationFlagsVertical;
    // Right rotation is tiledRotationFlagsHorizontal + tiledRotationFlagsDiagonal
    const unsigned long right = tiledRotationFlagsHorizontal + tiledRotationFlagsDiagonal;
    // Top rotation is tiledRotationFlagsHorizontal + tiledRotationFlagsVertical
    const unsigned long top = tiledRotationFlagsHorizontal + tiledRotationFlagsVertical;
};

class LevelManager
{
    public:
        explicit LevelManager(EntityManager& entityManager);

        Level& loadLevel();

        static inline std::shared_ptr<sf::Texture> tileSheetTexture;

        static inline Level activeLevel;

    private:
        Level loadMapData();
        void buildTileSheet(const char* tileSheetFilePath);
        std::vector<Tile> createTilesForWorld(const Level& level, const nlohmann::json& data);
        Tile& getTile(Level& level, uint32_t x, uint32_t y);
        uint32_t getPositionForTile(const Level& level, uint32_t x, uint32_t y);

    private:
        static inline TextureManager m_textureManager;

        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_LEVEL_MANAGER_H
