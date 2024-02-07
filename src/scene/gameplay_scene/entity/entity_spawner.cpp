#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager) : m_entityManager(entityManager)
{
}

void EntitySpawner::createPlayer()
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::PLAYER);

    std::shared_ptr<Crucible::Vec2> position = std::make_shared<Crucible::Vec2>(2 * Crucible::TILE_SIZE, 4 * Crucible::TILE_SIZE);

    auto& playerTransform = e.addComponent<Component::CTransform>(position);

    std::shared_ptr<sf::VertexArray> vertices = std::make_shared<sf::VertexArray>(sf::Quads);
    vertices->append(sf::Vertex({playerTransform.position->x, playerTransform.position->y}));
    vertices->append(sf::Vertex({playerTransform.position->x + Crucible::TILE_SIZE, playerTransform.position->y}));
    vertices->append(sf::Vertex({playerTransform.position->x + Crucible::TILE_SIZE, playerTransform.position->y + Crucible::TILE_SIZE}));
    vertices->append(sf::Vertex({playerTransform.position->x, playerTransform.position->y + Crucible::TILE_SIZE}));

    Tile playerTile(
            {static_cast<unsigned int>(position->x), static_cast<unsigned int>(position->y)},
            TileType::CENTRAL_WALL_LARGE_BROKEN_PURPLE,
            TileRotation::NONE,
            vertices);

    updateTileTexture(playerTile, LevelManager::dungeonTileSheetTexture);

    e.addComponent<Component::CControllable>();
    e.addComponent<Component::CCollider>();
    e.addComponent<Component::CTile>(playerTile);
}

void EntitySpawner::createGuard(const std::string& lightingObjectLayerName, const std::string& pathingObjectLayerName)
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::GUARD);
    ObjectLayer pathingObjectLayer = LevelManager::activeLevel.layerNameToObjectLayer.at(pathingObjectLayerName);

    std::vector<Crucible::Vec2> path;
    for (auto& guardPath : pathingObjectLayer.lightingObjectData)
    {
        sf::VertexArray& objectVertData = *guardPath.objectVertices;
        sf::Vertex v{objectVertData[0]};
        path.emplace_back(v.position.x, v.position.y);
    }

    e.addComponent<Component::CPathFollower>(path, pathingObjectLayerName);

    std::shared_ptr<Crucible::Vec2> position = std::make_shared<Crucible::Vec2>(path.at(0));

    auto& transform = e.addComponent<Component::CTransform>(position);
    std::shared_ptr<sf::VertexArray> vertices = std::make_shared<sf::VertexArray>(sf::Quads);
    vertices->append(sf::Vertex({transform.position->x, transform.position->y}));
    vertices->append(sf::Vertex({transform.position->x + Crucible::TILE_SIZE, transform.position->y}));
    vertices->append(sf::Vertex({transform.position->x + Crucible::TILE_SIZE, transform.position->y + Crucible::TILE_SIZE}));
    vertices->append(sf::Vertex({transform.position->x, transform.position->y + Crucible::TILE_SIZE}));

    Tile guardTile(
            {static_cast<unsigned int>(position->x), static_cast<unsigned int>(position->y)},
            TileType::CENTRAL_WALL_LARGE_BROKEN_PURPLE,
            TileRotation::NONE,
            vertices);

    updateTileTexture(guardTile, LevelManager::dungeonTileSheetTexture);
    e.addComponent<Component::CTile>(guardTile);
    e.addComponent<Component::CCollider>();

    std::vector<Crucible::Ray> rays = createRays(transform, lightingObjectLayerName);
    std::vector<std::vector<Crucible::LightRayIntersect>> defaultLightRayIntersects =
            std::vector<std::vector<Crucible::LightRayIntersect>>(rays.size(), std::vector<Crucible::LightRayIntersect>());
    e.addComponent<Component::CLightSource>(rays, sf::VertexArray(), defaultLightRayIntersects, lightingObjectLayerName);
}

void EntitySpawner::createTile(Tile& t, bool isCollidable, bool immovable)
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::TILE);
    Crucible::Vec2 position{static_cast<float>(t.position.x), static_cast<float>(t.position.y)};

    e.addComponent<Component::CTransform>(std::make_shared<Crucible::Vec2>(position));
    if (isCollidable)
    {
        e.addComponent<Component::CCollider>(immovable);
    }

    sf::VertexArray vertices(sf::Quads);
    vertices.append(sf::Vertex({position.x, position.y}));
    vertices.append(sf::Vertex({position.x + Crucible::TILE_SIZE, position.y}));
    vertices.append(sf::Vertex({position.x + Crucible::TILE_SIZE, position.y + Crucible::TILE_SIZE}));
    vertices.append(sf::Vertex({position.x, position.y + Crucible::TILE_SIZE}));

//    if (t.type == TileType::ARROW_BLOCK)
//    {
//        LevelManager::activeLevel.layerNameToObjectLayer[0].tileObjectVertices.emplace_back(vertices);
//    }

    t.vertices = std::make_shared<sf::VertexArray>(vertices);

    if (t.type == TileType::SPAWN_ZONE || t.type == TileType::END_ZONE)
    {
        updateTileTexture(t, LevelManager::basicTileSheetTexture);
    }
    else
    {
        updateTileTexture(t, LevelManager::dungeonTileSheetTexture);
    }
    e.addComponent<Component::CTile>(t);
}

std::vector<Crucible::Ray> EntitySpawner::createRays(Component::CTransform& playerTransform, const std::string& layerName)
{
    std::vector<Crucible::Ray> rays = std::vector<Crucible::Ray>();
    std::vector<Crucible::Ray> additionalRays = std::vector<Crucible::Ray>();

    ObjectLayer& objectLayer = LevelManager::activeLevel.layerNameToObjectLayer.at(layerName);

    for (const Object& tileObject : objectLayer.lightingObjectData)
    {
        std::shared_ptr<sf::VertexArray> tileObjectVertices = tileObject.objectVertices;
        for (size_t i = 0; i < tileObjectVertices->getVertexCount(); i++)
        {
            const sf::Vertex& v = (*tileObjectVertices)[i];
            // Add core ray
            rays.emplace_back(playerTransform.position, Crucible::Vec2(v.position.x, v.position.y));
            // Add additional rays to left and right of core ray (This happens in RayAppenderSystem)
            additionalRays.emplace_back(playerTransform.position, Crucible::Vec2(0, 0));
            additionalRays.emplace_back(playerTransform.position, Crucible::Vec2(0, 0));
        }
    }

    rays.insert(rays.end(), additionalRays.begin(), additionalRays.end());
    std::cout << "Configured: [" << rays.size() << "] light rays" << '\n';
    return rays;
}

/**
 * rotation guide:
 * None: [top-left vertex -> bottom-left vertex, closewise]
 * 90 degree anti-clockwise texture rotation: [top-right vertex -> top-left vertex, clockwise]
 * 90 degree clockwise texture rotation: [bottom-left vertex -> bottom-right vertex, clockwise]
 * flip horizontally: [top-right -> bottom-right, anti-clockwise]
 */
void EntitySpawner::updateTileTexture(Tile& tile, const std::shared_ptr<sf::Texture>& tileSheetTexture)
{
    sf::VertexArray& tileVertices = *tile.vertices;
    assert(tileVertices.getVertexCount() == 4);

    int tileTypeValue = static_cast<int>(tile.type) - 1;
    float tu = (tileTypeValue % (tileSheetTexture->getSize().x / Crucible::TILE_SIZE));
    float tv = tileTypeValue / (tileSheetTexture->getSize().x / Crucible::TILE_SIZE);

    float tuPositionStart = tu * Crucible::TILE_SIZE;
    float tuPositionEnd = (tu + 1) * Crucible::TILE_SIZE;
    float tvPositionStart = tv * Crucible::TILE_SIZE;
    float tvPositionEnd = (tv + 1) * Crucible::TILE_SIZE;

    if (tile.rotation == TileRotation::NONE)
    {
        tileVertices[0].texCoords = {tuPositionStart, tvPositionStart};
        tileVertices[1].texCoords = {tuPositionEnd, tvPositionStart};
        tileVertices[2].texCoords = {tuPositionEnd, tvPositionEnd};
        tileVertices[3].texCoords = {tuPositionStart, tvPositionEnd};
        return;
    }

    if (tile.rotation == TileRotation::FLIPPED_HORIZONTALLY)
    {
        tileVertices[0].texCoords = sf::Vector2f(tuPositionEnd, tvPositionStart);
        tileVertices[1].texCoords = sf::Vector2f(tuPositionStart, tvPositionStart);
        tileVertices[2].texCoords = sf::Vector2f(tuPositionStart, tvPositionEnd);
        tileVertices[3].texCoords = sf::Vector2f(tuPositionEnd, tvPositionEnd);
        return;
    }
}
