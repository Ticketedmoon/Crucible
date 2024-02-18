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

    public:
        static inline const std::string PLAYER_SPRITE_SHEET_PATH = "resources/assets/texture/player_crucible_16x32_6pr.png";
        static inline const std::string CATACOMB_TILESET_PATH = "resources/maps/catacomb/mainlevbuild.png";
        static inline const std::string MAP_DATA_PATH = "resources/maps/catacomb/catacombs.json";

        static inline const std::string COLLISION_LAYER_PLAYER_A = "collision_layer_player_a";
        static inline const std::string COLLISION_LAYER_PLAYER_B = "collision_layer_player_b";
        static inline const std::string GUARD_PATHING_LAYER_A = "guard_pathing_layer_player_a";
        static inline const std::string GUARD_PATHING_LAYER_B = "guard_pathing_layer_player_b";

        static inline Level activeLevel;

    private:
        void loadLevel();
        Level loadMapData();
        void loadTexture(const std::string& tileSheetFilePath);
        sf::VertexArray createTilesForWorld(const Level& level, const nlohmann::json& data, const size_t layerIdx);
        static uint32_t getPositionForTile(const Level& level, uint32_t x, uint32_t y);
        size_t lookupTileTypeForObject(size_t layerIdx, size_t i, nlohmann::json& data) const;

    private:
        const std::string LEVEL_FILE_LAYERS_KEY = "layers";
        const std::string LEVEL_FILE_TILESETS_KEY = "tilesets";
        const std::string LEVEL_FILE_TILES_KEY = "tiles";
        const std::string LEVEL_FILE_OBJECTS_KEY = "objects";
        const std::string LEVEL_FILE_DATA_KEY = "data";

        static inline const std::unordered_map<std::string, size_t> TILE_TYPE_LOOKUP_TABLE;

        EntityManager& m_entityManager;
        TextureManager& m_textureManager;
};


#endif //CRUCIBLE_LEVEL_MANAGER_H
