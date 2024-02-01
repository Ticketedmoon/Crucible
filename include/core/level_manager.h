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
        std::vector<Tile> createTilesForWorld(const Level& level, const nlohmann::json& data, const size_t layerIdx);
        Tile& getTile(Level& level, uint32_t x, uint32_t y);
        uint32_t getPositionForTile(const Level& level, uint32_t x, uint32_t y);

    private:
        const char* LEVEL_FILE_LAYERS_KEY = "layers";
        const char* LEVEL_FILE_OBJECTS_KEY = "objects";
        const char* LEVEL_FILE_DATA_KEY = "data";

        static inline TextureManager m_textureManager;

        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_LEVEL_MANAGER_H
