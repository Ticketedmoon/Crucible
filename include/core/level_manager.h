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
        std::vector<Tile> createTilesForWorld(const Level& level, const nlohmann::json& data, size_t layerIdx);
        Tile& getTile(Level& level, uint32_t x, uint32_t y);
        uint32_t getPositionForTile(const Level& level, uint32_t x, uint32_t y);

    private:
        const char* tileSheetFilePath = "resources/maps/dungeon_bricks_shadow_tilset.png";
        const char* mapFilePath = "resources/maps/level_one_map_dungeon_tileset.json";

        const char* LEVEL_FILE_LAYERS_KEY = "layers";
        const char* LEVEL_FILE_TILESETS_KEY = "tilesets";
        const char* LEVEL_FILE_TILES_KEY = "tiles";
        const char* LEVEL_FILE_OBJECTS_KEY = "objects";
        const char* LEVEL_FILE_DATA_KEY = "data";

        static inline const std::unordered_map<std::string, TileType> TILE_TYPE_LOOKUP_TABLE = {
                {"BACKGROUND_PURPLE_WALL",            TileType::BACKGROUND_PURPLE_WALL},
                {"TOP_CORNER_WALL_BROKEN_PURPLE",     TileType::TOP_CORNER_WALL_BROKEN_PURPLE},
                {"TOP_WALL_BROKEN_PURPLE",            TileType::TOP_WALL_BROKEN_PURPLE},
                {"BORDER_WALL_BROKEN_PURPLE",         TileType::BORDER_WALL_BROKEN_PURPLE},
                {"BOTTOM_CORNER_WALL_BROKEN_PURPLE",  TileType::BOTTOM_CORNER_WALL_BROKEN_PURPLE},
                {"BOTTOM_WALL_BROKEN_PURPLE",         TileType::BOTTOM_WALL_BROKEN_PURPLE},
                {"COLUMN_WALL_BROKEN_PURPLE",         TileType::COLUMN_WALL_BROKEN_PURPLE},
                {"CENTRAL_WALL_SMALL_BROKEN_PURPLE",  TileType::CENTRAL_WALL_SMALL_BROKEN_PURPLE},
                {"CENTRAL_WALL_MEDIUM_BROKEN_PURPLE", TileType::CENTRAL_WALL_MEDIUM_BROKEN_PURPLE},
                {"CENTRAL_WALL_LARGE_BROKEN_PURPLE",  TileType::CENTRAL_WALL_LARGE_BROKEN_PURPLE},
        };

        static inline TextureManager m_textureManager;

        EntityManager& m_entityManager;
};


#endif //CRUCIBLE_LEVEL_MANAGER_H
