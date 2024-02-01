#include "level_manager.h"

LevelManager::LevelManager(EntityManager& entityManager) : m_entityManager(entityManager)
{
}

Level& LevelManager::loadLevel()
{
    // Level
    activeLevel = loadMapData();

    // Build Tile sheet
    const char* tileSheetFilePath = "resources/maps/basic_tileset.png";
    buildTileSheet(tileSheetFilePath);

    tileSheetTexture = m_textureManager.getTexture(tileSheetFilePath);

    return activeLevel;
}

Tile& LevelManager::getTile(Level& level, uint32_t x, uint32_t y)
{
    uint32_t positionForTile = getPositionForTile(level, x, y);
    return level.layers[0].data.at(positionForTile);
}

uint32_t LevelManager::getPositionForTile(const Level& level, uint32_t x, uint32_t y)
{
    uint32_t row = level.width * y;
    uint32_t positionForTile = row + x;
    return positionForTile;
}

Level LevelManager::loadMapData()
{
    const char* mapFilePath = "resources/maps/level_one_map_basic_tileset.json";

    std::ifstream f(mapFilePath);
    nlohmann::json data = nlohmann::json::parse(f);

    Level level;

    level.width = data["width"];
    level.height = data["height"];

    TileLayer layerA;
    layerA.name = data["layers"][0]["name"];
    layerA.data = createTilesForWorld(level, data);

    for (size_t i = 0; i < data["layers"][1]["objects"].size(); i++)
    {
        sf::FloatRect r;
        r.left = data["layers"][1]["objects"][i]["x"];
        r.top = data["layers"][1]["objects"][i]["y"];
        r.width = data["layers"][1]["objects"][i]["width"];
        r.height = data["layers"][1]["objects"][i]["height"];

        std::shared_ptr<sf::VertexArray> verts = std::make_shared<sf::VertexArray>();
        verts->append({{r.left, r.top}});
        verts->append({{r.left + r.width, r.top}});
        verts->append({{r.left + r.width, r.top + r.height}});
        verts->append({{r.left, r.top + r.height}});
        verts->append({{r.left, r.top}});

        layerA.tileObjectsVertexLayer.emplace_back(verts);
    }

    level.layers.emplace_back(layerA);

    return level;
}

std::vector<Tile> LevelManager::createTilesForWorld(const Level& level, const nlohmann::json& data)
{
    std::vector<Tile> tiles;
    std::vector<uint8_t> tileValues = data["layers"][0]["data"].get<std::vector<uint8_t>>();
    for (unsigned int y = 0; y < level.height; y++)
    {
        for (unsigned int x = 0; x < level.width; x++)
        {
            uint32_t position = getPositionForTile(level, x, y);
            auto tileType = static_cast<TileType>(tileValues[position]);
            sf::Vector2u pos{x, y};
            Tile tile(pos, tileType, {});
            tiles.emplace_back(tile);
        }
    }
    return tiles;
}

void LevelManager::buildTileSheet(const char* tileSheetFilePath)
{
    if (!m_textureManager.hasTexture(tileSheetFilePath))
    {
        m_textureManager.addTexture(tileSheetFilePath);
    }
}
