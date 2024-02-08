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
        explicit LevelManager(EntityManager& entityManager, TextureManager& textureManager);

        Level& loadLevel();

    public:
        static inline const std::string PLAYER_SPRITE_SHEET_PATH = "resources/assets/texture/player_crucible_64x64_6pr.png";
        static inline const std::string BASIC_TILE_SHEET_PATH = "resources/maps/basic_tileset.png";
        static inline const std::string DUNGEON_TILE_SHEET_PATH = "resources/maps/dungeon_bricks_shadow_tileset.png";
        static inline const std::string MAP_DATA_PATH = "resources/maps/level_one_map_dungeon_tileset.json";

        static inline const std::string LIGHTING_OBJECT_LAYER_A_NAME = "lighting_object_layer_a";
        static inline const std::string LIGHTING_OBJECT_LAYER_B_NAME = "lighting_object_layer_b";
        static inline const std::string GUARD_PATHING_LAYER_A = "guard_pathing_layer_a";
        static inline const std::string GUARD_PATHING_LAYER_B = "guard_pathing_layer_b";

        static inline Level activeLevel;

    private:
        Level loadMapData();
        void loadTexture(const std::string& tileSheetFilePath);
        std::vector<Tile> createTilesForWorld(const Level& level, const nlohmann::json& data, size_t layerIdx);
        uint32_t getPositionForTile(const Level& level, uint32_t x, uint32_t y);
        TileType lookupTileTypeForObject(size_t layerIdx, size_t i, nlohmann::json& data) const;

    private:
        const std::string LEVEL_FILE_LAYERS_KEY = "layers";
        const std::string LEVEL_FILE_TILESETS_KEY = "tilesets";
        const std::string LEVEL_FILE_TILES_KEY = "tiles";
        const std::string LEVEL_FILE_OBJECTS_KEY = "objects";
        const std::string LEVEL_FILE_DATA_KEY = "data";

        static inline const std::unordered_map<std::string, TileType> TILE_TYPE_LOOKUP_TABLE = {
                {"SPAWN_ZONE",                        TileType::SPAWN_ZONE},
                {"END_ZONE",                          TileType::END_ZONE},
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

        EntityManager& m_entityManager;
        TextureManager& m_textureManager;
};


#endif //CRUCIBLE_LEVEL_MANAGER_H
