#include "level_manager.h"

LevelManager::LevelManager(EntityManager& entityManager, TextureManager& textureManager)
    : m_entityManager(entityManager), m_textureManager(textureManager)
{
}

Level& LevelManager::loadLevel()
{
    // Level
    activeLevel = loadMapData();

    // Build player sprite sheet
    loadTexture(PLAYER_SPRITE_SHEET_PATH);

    // Build Tile sheet
    loadTexture(BASIC_TILE_SHEET_PATH);
    loadTexture(DUNGEON_TILE_SHEET_PATH);

    return activeLevel;
}

uint32_t LevelManager::getPositionForTile(const Level& level, uint32_t x, uint32_t y)
{
    uint32_t row = level.width * y;
    uint32_t positionForTile = row + x;
    return positionForTile;
}

Level LevelManager::loadMapData()
{
    std::ifstream f(MAP_DATA_PATH);
    nlohmann::json data = nlohmann::json::parse(f);

    Level level;
    level.width = data["width"];
    level.height = data["height"];

    for (size_t layerIdx = 0; layerIdx < data[LEVEL_FILE_LAYERS_KEY].size(); layerIdx++)
    {
        std::string layerType = data[LEVEL_FILE_LAYERS_KEY][layerIdx]["type"];
        // TODO use enum class for this
        if (layerType == "tilelayer")
        {
            TileLayer layer;
            layer.name = data[LEVEL_FILE_LAYERS_KEY][layerIdx]["name"];
            layer.type = layerType;
            layer.data = createTilesForWorld(level, data, layerIdx);
            level.tileLayers.emplace_back(layer);
        }

        if (layerType == "objectgroup")
        {
            ObjectLayer layer;
            layer.name = data[LEVEL_FILE_LAYERS_KEY][layerIdx]["name"];
            layer.type = layerType;

            for (size_t i = 0; i < data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY].size(); i++)
            {
                sf::FloatRect r;
                r.left = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i]["x"];
                r.top = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i]["y"];
                r.width = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i]["width"];
                r.height = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i]["height"];

                std::shared_ptr<sf::VertexArray> verts = std::make_shared<sf::VertexArray>();
                verts->append({{r.left, r.top}});
                verts->append({{r.left + r.width, r.top}});
                verts->append({{r.left + r.width, r.top + r.height}});
                verts->append({{r.left, r.top + r.height}});
                // TODO @investigate: currently this vert is required, but it may not be necessary
                verts->append({{r.left, r.top}});

                TileType tileType = lookupTileTypeForObject(layerIdx, i, data);
                Object levelObject{Crucible::EntityType::TILE, tileType, verts};
                level.layerNameToObjectLayer[layer.name].lightingObjectData.emplace_back(levelObject);
            }
        }
    }

    return level;
}

TileType LevelManager::lookupTileTypeForObject(size_t layerIdx, size_t i, nlohmann::json& data) const
{
    TileType tileType{TileType::NONE};
    if (data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i].contains("properties"))
    {
        std::vector<std::unordered_map<std::string, std::string>> properties =
                data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i]["properties"];

        for (std::unordered_map<std::string, std::string> p: properties)
        {
            if (p["name"] == "type")
            {
                tileType = TILE_TYPE_LOOKUP_TABLE.at(p["value"]);
                break;
            }
        }
    }
    return tileType;
}

std::vector<Tile> LevelManager::createTilesForWorld(const Level& level, const nlohmann::json& data, const size_t layerIdx)
{
    std::vector<Tile> tiles;
    std::vector<long> tileValues = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_DATA_KEY].get<std::vector<long>>();

    unsigned long firstGidDungeonTileset = data[LEVEL_FILE_TILESETS_KEY][0]["firstgid"].get<long>();
    unsigned long firstGidBasicTileset = data[LEVEL_FILE_TILESETS_KEY][1]["firstgid"].get<long>();

    for (unsigned int y = 0; y < level.height; y++)
    {
        for (unsigned int x = 0; x < level.width; x++)
        {
            uint32_t position = getPositionForTile(level, x, y);
            unsigned long globalTileId = tileValues[position];

            // Read out the flags
            bool flippedHorizontally = (globalTileId & Crucible::FLIPPED_HORIZONTALLY_FLAG);
            bool flippedVertically = (globalTileId & Crucible::FLIPPED_VERTICALLY_FLAG);
            bool flippedDiagonally = (globalTileId & Crucible::FLIPPED_DIAGONALLY_FLAG);
            bool rotatedHex120 = (globalTileId & Crucible::ROTATED_HEXAGONAL_120_FLAG);

            // Clear all four flags
            globalTileId &= ~(Crucible::FLIPPED_HORIZONTALLY_FLAG |
                    Crucible::FLIPPED_VERTICALLY_FLAG |
                    Crucible::FLIPPED_DIAGONALLY_FLAG |
                    Crucible::ROTATED_HEXAGONAL_120_FLAG);

            TileRotation rotation = flippedHorizontally ? TileRotation::FLIPPED_HORIZONTALLY
                    : flippedVertically ? TileRotation::FLIPPED_VERTICALLY
                    : TileRotation::NONE;

            unsigned long localTileId = globalTileId >= firstGidBasicTileset
                    ? (globalTileId - firstGidBasicTileset) + 1
                    : (globalTileId - firstGidDungeonTileset) + 1;

            auto tileType = static_cast<TileType>(localTileId);
            sf::Vector2u pos{x, y};
            Tile tile(pos, tileType, rotation, {});
            tiles.emplace_back(tile);
        }
    }
    return tiles;
}

void LevelManager::loadTexture(const std::string& tileSheetFilePath)
{
    if (!m_textureManager.hasTexture(tileSheetFilePath))
    {
        m_textureManager.addTexture(tileSheetFilePath);
    }
}
