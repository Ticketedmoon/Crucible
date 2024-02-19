#include "level_manager.h"

LevelManager::LevelManager(EntityManager& entityManager, TextureManager& textureManager)
    : m_entityManager(entityManager), m_textureManager(textureManager)
{
    loadTexture(PLAYER_SPRITE_SHEET_PATH);
    loadTexture(CATACOMB_MAIN_TILESET_PATH);
    loadTexture(CATACOMB_DECORATIVE_TILESET_PATH);
    loadTexture(CATACOMB_TORCH_TILESET_PATH);

    // Level
    activeLevel = loadMapData();
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
            layer.tilesetPathToLevelData = createTilesForWorld(level, data, layerIdx);
            level.tileLayers.emplace_back(layer);
        }

        if (layerType == "objectgroup")
        {
            ObjectLayer layer;
            layer.name = data[LEVEL_FILE_LAYERS_KEY][layerIdx]["name"];
            layer.type = layerType;

            size_t totalObjectsForLayer = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY].size();
            for (size_t i = 0; i < totalObjectsForLayer; i++)
            {
                const char *pointKey = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i].contains("polygon")
                        ? "polygon"
                        : "polyline";

                float parentX = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i]["x"];
                float parentY = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i]["y"];
                for (size_t j = 0; j < data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i][pointKey].size(); j++)
                {
                    sf::FloatRect r;
                    float polygonPointX = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i][pointKey][j]["x"];
                    float polygonPointY = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i][pointKey][j]["y"];

                    r.left = parentX + polygonPointX;
                    r.top = parentY + polygonPointY;
                    r.width = 0;
                    r.height = 0;

                    std::shared_ptr<sf::VertexArray> verts = std::make_shared<sf::VertexArray>();
                    verts->append({{r.left, r.top}});
                    verts->append({{r.left + r.width, r.top}});
                    verts->append({{r.left + r.width, r.top + r.height}});
                    verts->append({{r.left, r.top + r.height}});
                    // TODO @investigate: currently this vert is required, but it may not be necessary
                    verts->append({{r.left, r.top}});

                    size_t tileTypeIdx = lookupTileTypeForObject(layerIdx, j, data);
                    Object levelObject{Crucible::EntityType::TILE, tileTypeIdx, verts};
                    level.layerNameToObjectLayer[layer.name].lightingObjectData.emplace_back(levelObject);
                }

                if (data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i].contains("properties"))
                {
                    // TODO this points to 0th index - make scalable in future
                    const std::string& name = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i]["properties"][0]["name"];
                    const std::string& type = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i]["properties"][0]["type"];
                    const std::string& value = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i]["properties"][0]["value"];

                    if (layer.customProperties.contains(name))
                    {
                        layer.customProperties.at(name).emplace_back(name, type, value);
                    }
                    else
                    {
                        layer.customProperties[name] = {{name, type, value}};
                    }
                }
            }
        }
    }

    return level;
}

size_t LevelManager::lookupTileTypeForObject(size_t layerIdx, size_t i, nlohmann::json& data) const
{
    if (!data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i].contains("properties"))
    {
        return -1;
    }

    std::vector<std::unordered_map<std::string, std::string>> properties =
            data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY][i]["properties"];

    for (std::unordered_map<std::string, std::string> p: properties)
    {
        if (p["name"] == "type")
        {
            return TILE_TYPE_LOOKUP_TABLE.at(p["value"]);
        }
    }
}

std::unordered_map<std::string, sf::VertexArray> LevelManager::createTilesForWorld(
        const Level& level,
        const nlohmann::json& data,
        const size_t layerIdx)
{
    std::unordered_map<std::string, sf::VertexArray> tilesetPathToVertexArrayForLayer{
            {CATACOMB_MAIN_TILESET_PATH, sf::VertexArray(sf::Quads)},
            {CATACOMB_DECORATIVE_TILESET_PATH, sf::VertexArray(sf::Quads)},
            {CATACOMB_TORCH_TILESET_PATH, sf::VertexArray(sf::Quads)}
    };
    std::vector<long> tileValues = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_DATA_KEY].get<std::vector<long>>();

    // TODO make dynamic
    unsigned long mainTilesetFirstGid = data[LEVEL_FILE_TILESETS_KEY][0]["firstgid"].get<long>();
    unsigned long decorativeTilesetFirstGid = data[LEVEL_FILE_TILESETS_KEY][1]["firstgid"].get<long>();
    unsigned long torchTilesetFirstGid = data[LEVEL_FILE_TILESETS_KEY][2]["firstgid"].get<long>();

    sf::Vector2u tileDimensions{Crucible::TILE_SIZE, Crucible::TILE_SIZE};

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

            bool rotatedLeft = (globalTileId & Crucible::FLIPPED_LEFT_FLAG);
            bool rotatedRight = (globalTileId & Crucible::FLIPPED_RIGHT_FLAG);
            bool rotatedTop = (globalTileId & Crucible::FLIPPED_TOP_FLAG);
            // Left rotation is diagonal + vertical

            // Clear all four flags
            globalTileId &= ~(Crucible::FLIPPED_HORIZONTALLY_FLAG |
                    Crucible::FLIPPED_VERTICALLY_FLAG |
                    Crucible::FLIPPED_DIAGONALLY_FLAG |
                    Crucible::ROTATED_HEXAGONAL_120_FLAG);

            unsigned long localTileId = globalTileId >= torchTilesetFirstGid
                    ? globalTileId - torchTilesetFirstGid
                    : globalTileId >= decorativeTilesetFirstGid
                            ? globalTileId - decorativeTilesetFirstGid
                            : globalTileId - mainTilesetFirstGid;

            sf::Vector2f pos{
                static_cast<float>(x * Crucible::TILE_SIZE),
                static_cast<float>(y * Crucible::TILE_SIZE)
            };

            sf::Vertex vertexA = sf::Vertex({pos.x, pos.y});
            sf::Vertex vertexB = sf::Vertex({pos.x + tileDimensions.x, pos.y});
            sf::Vertex vertexC = sf::Vertex({pos.x + tileDimensions.x, pos.y + tileDimensions.y});
            sf::Vertex vertexD = sf::Vertex({pos.x, pos.y + tileDimensions.y});

            const std::string& texturePath = globalTileId >= torchTilesetFirstGid
                    ? CATACOMB_TORCH_TILESET_PATH
                    : globalTileId >= decorativeTilesetFirstGid
                            ? CATACOMB_DECORATIVE_TILESET_PATH
                            : CATACOMB_MAIN_TILESET_PATH;

            std::shared_ptr<sf::Texture>& tileSheetTexture = m_textureManager.getTexture(texturePath);
            float tu = (localTileId % (tileSheetTexture->getSize().x / tileDimensions.x));
            float tv = (localTileId / (tileSheetTexture->getSize().x /tileDimensions.y));

            float tuPositionStart = tu * tileDimensions.x;
            float tuPositionEnd = (tu + 1) * tileDimensions.x;
            float tvPositionStart = tv * tileDimensions.y;
            float tvPositionEnd = (tv + 1) * tileDimensions.y;

            if (rotatedLeft)
            {
                vertexA.texCoords = {tuPositionEnd, tvPositionStart};
                vertexB.texCoords = {tuPositionEnd, tvPositionEnd};
                vertexC.texCoords = {tuPositionStart, tvPositionEnd};
                vertexD.texCoords = {tuPositionStart, tvPositionStart};
            }
            else if (rotatedRight)
            {

            }
            else if (rotatedTop)
            {

            }
            else if (flippedHorizontally)
            {
                vertexA.texCoords = {tuPositionEnd, tvPositionStart};
                vertexB.texCoords = {tuPositionStart, tvPositionStart};
                vertexC.texCoords = {tuPositionStart, tvPositionEnd};
                vertexD.texCoords = {tuPositionEnd, tvPositionEnd};
            }
            else if (flippedVertically)
            {

            }
            else
            {
                vertexA.texCoords = {tuPositionStart, tvPositionStart};
                vertexB.texCoords = {tuPositionEnd, tvPositionStart};
                vertexC.texCoords = {tuPositionEnd, tvPositionEnd};
                vertexD.texCoords = {tuPositionStart, tvPositionEnd};
            }

            tilesetPathToVertexArrayForLayer.at(texturePath).append(vertexA);
            tilesetPathToVertexArrayForLayer.at(texturePath).append(vertexB);
            tilesetPathToVertexArrayForLayer.at(texturePath).append(vertexC);
            tilesetPathToVertexArrayForLayer.at(texturePath).append(vertexD);
        }
    }

    return tilesetPathToVertexArrayForLayer;
}

uint32_t LevelManager::getPositionForTile(const Level& level, uint32_t x, uint32_t y)
{
    uint32_t row = level.width * y;
    uint32_t positionForTile = row + x;
    return positionForTile;
}

void LevelManager::loadTexture(const std::string& tileSheetFilePath)
{
    if (!m_textureManager.hasTexture(tileSheetFilePath))
    {
        m_textureManager.addTexture(tileSheetFilePath);
    }
}
