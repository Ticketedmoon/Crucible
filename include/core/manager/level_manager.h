#pragma once

#ifndef CRUCIBLE_LEVEL_MANAGER_H
#define CRUCIBLE_LEVEL_MANAGER_H

#include <json.hpp>
#include <fstream>
#include <iostream>

#include <SFML/Graphics/Texture.hpp>

#include "tile.h"
#include "core/level/level.h"
#include "texture_manager.h"
#include "core/engine/common_constants.h"
#include "entity_manager.h"

class LevelManager
{
    public:
        explicit LevelManager(TextureManager& textureManager);

    public:
        static inline const std::string TILESET_ROOT_PATH = "resources/maps/catacomb/";

        // Can this be acquired programmatically like the tilesets?
        static inline const std::string MAP_DATA_PATH = "resources/maps/catacomb/catacombs.json";

        static inline const std::string COLLISION_LAYER_PLAYER_A = "collision_layer_player_a";
        static inline const std::string COLLISION_LAYER_PLAYER_B = "collision_layer_player_b";
        static inline const std::string GUARD_LIGHTING_LAYER_A = "guard_lighting_layer_a";
        static inline const std::string GUARD_PATHING_LAYER_A = "guard_pathing_layer_a";
        static inline const std::string GUARD_PATHING_LAYER_B = "guard_pathing_layer_b";

        static inline Level activeLevel;

    private:
        void loadMapData(nlohmann::json data);
        void loadTileSets(nlohmann::json data);
        void loadTexturesForLevel();

        void addObjectsToLayer(const nlohmann::json& data, size_t layerIdx, ObjectLayer& objectLayer);
        static void addCustomPropertiesToLayer(ObjectLayer& objectLayer, nlohmann::basic_json<>& json);

        std::unordered_map<std::string, sf::VertexArray> createTilesForWorld(
                const nlohmann::json& data,
                size_t layerIdx);

        static uint32_t getPositionForTile(uint32_t x, uint32_t y);

        static std::pair<std::string, unsigned long> findLocalTilesetId(
                const std::vector<TileSet>& orderedTileSets,
                std::unordered_map<std::string,
                unsigned long>& tilesetPathToFirstGids,
                unsigned long globalTileId);

        void setTextureCoordinatesForQuad(const sf::Vector2u& tileDimensions, bool flippedHorizontally,
                bool flippedVertically,
                bool rotatedLeft, bool rotatedRight, bool rotatedTop,
                const std::pair<std::string, unsigned long>& tilePathToLocalTileIdPair, sf::Vertex& vertexA,
                sf::Vertex& vertexB, sf::Vertex& vertexC, sf::Vertex& vertexD);

    private:
        const std::string LEVEL_FILE_LAYERS_KEY = "layers";
        const std::string LEVEL_FILE_TILESETS_KEY = "tilesets";
        const std::string LEVEL_FILE_OBJECTS_KEY = "objects";
        const std::string LEVEL_FILE_DATA_KEY = "data";

        TextureManager& m_textureManager;
};


#endif //CRUCIBLE_LEVEL_MANAGER_H
