#include "level_manager.h"

LevelManager::LevelManager(EntityManager& entityManager, TextureManager& textureManager)
    : m_entityManager(entityManager), m_textureManager(textureManager)
{
    std::ifstream f(MAP_DATA_PATH);
    nlohmann::json data = nlohmann::json::parse(f);

    activeLevel.width = data["width"];
    activeLevel.height = data["height"];

    loadTileSets(data);
    loadTexturesForLevel();
    loadMapData(data);
}

void LevelManager::loadTexturesForLevel()
{
    loadTexture(PLAYER_SPRITE_SHEET_PATH);
    for (const TileSet& tileSet : activeLevel.tileSets)
    {
        loadTexture(tileSet.path);
    }
}

void LevelManager::loadTileSets(nlohmann::json data)
{
    for (size_t tilesetIdx = 0; tilesetIdx < data[LEVEL_FILE_TILESETS_KEY].size(); tilesetIdx++)
    {
        TileSet tileset{
                data[LEVEL_FILE_TILESETS_KEY][tilesetIdx]["firstgid"].get<size_t>(),
                data[LEVEL_FILE_TILESETS_KEY][tilesetIdx]["name"].get<std::string>(),
                TILESET_ROOT_PATH + data[LEVEL_FILE_TILESETS_KEY][tilesetIdx]["image"].get<std::string>(),
                data[LEVEL_FILE_TILESETS_KEY][tilesetIdx]["tilecount"].get<size_t>(),
                data[LEVEL_FILE_TILESETS_KEY][tilesetIdx]["tilewidth"].get<size_t>(),
                data[LEVEL_FILE_TILESETS_KEY][tilesetIdx]["tileheight"].get<size_t>(),
                data[LEVEL_FILE_TILESETS_KEY][tilesetIdx]["columns"].get<size_t>()
        };
        activeLevel.tileSets.emplace_back(tileset);
    }
}

void LevelManager::loadMapData(nlohmann::json data)
{

    for (size_t layerIdx = 0; layerIdx < data[LEVEL_FILE_LAYERS_KEY].size(); layerIdx++)
    {
        std::string layerType = data[LEVEL_FILE_LAYERS_KEY][layerIdx]["type"];
        // TODO use enum class for this
        if (layerType == "tilelayer")
        {
            TileLayer layer;
            layer.name = data[LEVEL_FILE_LAYERS_KEY][layerIdx]["name"];
            layer.type = layerType;
            layer.tilesetPathToLevelData = createTilesForWorld(data, layerIdx);
            activeLevel.tileLayers.emplace_back(layer);
        }

        if (layerType == "objectgroup")
        {
            ObjectLayer layer;
            layer.name = data[LEVEL_FILE_LAYERS_KEY][layerIdx]["name"];
            layer.type = layerType;

            size_t totalObjectsForLayer = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_OBJECTS_KEY].size();
            for (size_t i = 0; i < totalObjectsForLayer; i++)
            {
                addPolygonObjectsToLayer(data, layerIdx, layer, i);
                addCustomPropertiesToLayer(data, layerIdx, layer, i);
            }
        }
    }
}
void LevelManager::addPolygonObjectsToLayer(const nlohmann::json& data, size_t layerIdx, const ObjectLayer& layer, size_t i)
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

        Object levelObject{Crucible::EntityType::LEVEL_OBJECT, verts};
        activeLevel.layerNameToObjectLayer[layer.name].lightingObjectData.emplace_back(levelObject);
    }
}

void LevelManager::addCustomPropertiesToLayer(
        const nlohmann::json& data,
        size_t layerIdx,
        ObjectLayer& layer,
        size_t i) const
{
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

std::unordered_map<std::string, sf::VertexArray> LevelManager::createTilesForWorld(
        const nlohmann::json& data,
        const size_t layerIdx)
{
    std::unordered_map<std::string, unsigned long> tilesetPathToFirstGids;
    std::unordered_map<std::string, sf::VertexArray> tilesetPathToVertexArrayForLayer;
    for (const TileSet& tileSet : activeLevel.tileSets)
    {
        tilesetPathToVertexArrayForLayer.insert({tileSet.path, sf::VertexArray(sf::Quads)});
        tilesetPathToFirstGids.insert({tileSet.path, tileSet.firstgid});
    }

    sf::Vector2u tileDimensions{Crucible::TILE_SIZE, Crucible::TILE_SIZE};
    std::vector<long> tileValues = data[LEVEL_FILE_LAYERS_KEY][layerIdx][LEVEL_FILE_DATA_KEY].get<std::vector<long>>();

    for (unsigned int y = 0; y < activeLevel.height; y++)
    {
        for (unsigned int x = 0; x < activeLevel.width; x++)
        {
            uint32_t position = getPositionForTile(x, y);
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

            std::pair<std::string, unsigned long> tilePathToLocalTileIdPair
                = findLocalTilesetId(activeLevel.tileSets, tilesetPathToFirstGids, globalTileId);

            sf::Vector2f pos{
                static_cast<float>(x * Crucible::TILE_SIZE),
                static_cast<float>(y * Crucible::TILE_SIZE)
            };

            sf::Vertex vertexA = sf::Vertex({pos.x, pos.y});
            sf::Vertex vertexB = sf::Vertex({pos.x + tileDimensions.x, pos.y});
            sf::Vertex vertexC = sf::Vertex({pos.x + tileDimensions.x, pos.y + tileDimensions.y});
            sf::Vertex vertexD = sf::Vertex({pos.x, pos.y + tileDimensions.y});

            setTextureCoordinatesForQuad(tileDimensions, flippedHorizontally, flippedVertically, rotatedLeft,
                    rotatedRight, rotatedTop,
                    tilePathToLocalTileIdPair, vertexA, vertexB, vertexC, vertexD);

            tilesetPathToVertexArrayForLayer.at(tilePathToLocalTileIdPair.first).append(vertexA);
            tilesetPathToVertexArrayForLayer.at(tilePathToLocalTileIdPair.first).append(vertexB);
            tilesetPathToVertexArrayForLayer.at(tilePathToLocalTileIdPair.first).append(vertexC);
            tilesetPathToVertexArrayForLayer.at(tilePathToLocalTileIdPair.first).append(vertexD);
        }
    }

    return tilesetPathToVertexArrayForLayer;
}
void LevelManager::setTextureCoordinatesForQuad(const sf::Vector2u& tileDimensions, bool flippedHorizontally,
        bool flippedVertically, bool rotatedLeft, bool rotatedRight, bool rotatedTop,
        const std::pair<std::string, unsigned long>& tilePathToLocalTileIdPair, sf::Vertex& vertexA,
        sf::Vertex& vertexB, sf::Vertex& vertexC, sf::Vertex& vertexD)
{
    std::shared_ptr<sf::Texture>& tileSheetTexture = m_textureManager.getTexture(tilePathToLocalTileIdPair.first);
    float tu = (tilePathToLocalTileIdPair.second % (tileSheetTexture->getSize().x / tileDimensions.x));
    float tv = (tilePathToLocalTileIdPair.second / (tileSheetTexture->getSize().x /tileDimensions.y));

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
}

std::pair<std::string, unsigned long> LevelManager::findLocalTilesetId(
        const std::vector<TileSet>& tileSets,
        std::unordered_map<std::string,
        unsigned long>& tilesetPathToFirstGids,
        unsigned long globalTileId)
{
    for (size_t i = tileSets.size(); i > 0; i--)
    {
        const std::string& tileSetPath = tileSets.at(i-1).path;
        long layerGid = tilesetPathToFirstGids.at(tileSetPath);
        if (globalTileId >= layerGid - 1)
        {
            const size_t localTileId = globalTileId - layerGid;
            return std::make_pair(tileSetPath, localTileId);
        }
    }

    throw std::runtime_error("Failed to find local tileset for globalTileId: " + std::to_string(globalTileId));
}

uint32_t LevelManager::getPositionForTile(uint32_t x, uint32_t y)
{
    uint32_t row = activeLevel.width * y;
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
