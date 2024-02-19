#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager, TextureManager& textureManager)
    : m_entityManager(entityManager), m_textureManager(textureManager)
{
}

void EntitySpawner::createPlayer()
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::PLAYER);

    Crucible::Vec2 playerDimensions{Crucible::TILE_SIZE, Crucible::TILE_SIZE};
    std::shared_ptr<Crucible::Vec2> position = std::make_shared<Crucible::Vec2>(Crucible::PLAYER_SPAWN_LOCATIONS[0]);

    auto& playerTransform = e.addComponent<Component::CTransform>(position, playerDimensions);

    std::shared_ptr<sf::VertexArray> vertices = std::make_shared<sf::VertexArray>(sf::Quads);
    vertices->append(sf::Vertex({playerTransform.position->x, playerTransform.position->y}));
    vertices->append(sf::Vertex({playerTransform.position->x + playerDimensions.x, playerTransform.position->y}));
    vertices->append(sf::Vertex({playerTransform.position->x + playerDimensions.x, playerTransform.position->y + playerDimensions.y}));
    vertices->append(sf::Vertex({playerTransform.position->x, playerTransform.position->y + playerDimensions.y}));

    Tile playerTile(
            {static_cast<unsigned int>(position->x), static_cast<unsigned int>(position->y)},
            0,
            TileRotation::NONE,
            vertices);

    std::shared_ptr<sf::Texture>& texture = m_textureManager.getTexture(LevelManager::PLAYER_SPRITE_SHEET_PATH);

    e.addComponent<Component::CAnimation>(LevelManager::PLAYER_SPRITE_SHEET_PATH);
    e.addComponent<Component::CControllable>();
    e.addComponent<Component::CCollider>();
    e.addComponent<Component::CTile>(playerTile, texture);
}

void EntitySpawner::createGuard(const std::string& lightingObjectLayerName, const std::string& pathingObjectLayerName)
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::GUARD);
    ObjectLayer pathingObjectLayer = LevelManager::activeLevel.layerNameToObjectLayer.at(pathingObjectLayerName);

    std::vector<Waypoint> path;
    for (size_t i = 0; i < pathingObjectLayer.lightingObjectData.size(); i++)
    {
        Object& guardPath = pathingObjectLayer.lightingObjectData[i];
        sf::VertexArray& objectVertData = *guardPath.objectVertices;
        sf::Vertex v{objectVertData[0]};

        bool polygonPointAtIndexHasWaitPeriod =
            pathingObjectLayer.customProperties.contains("point_idx") &&
            std::stoi(pathingObjectLayer.customProperties.at("point_idx").at(0).value) == i
                && pathingObjectLayer.customProperties.contains("wait_period") &&
                std::stoi(pathingObjectLayer.customProperties.at("wait_period").at(0).value);

        uint32_t waitPeriodMs = pathingObjectLayer.customProperties.contains("wait_period")
                ? polygonPointAtIndexHasWaitPeriod
                : 0;
        path.emplace_back(Waypoint({v.position.x, v.position.y}, waitPeriodMs));
    }

    e.addComponent<Component::CPathFollower>(path, pathingObjectLayerName);

    std::shared_ptr<Crucible::Vec2> position = std::make_shared<Crucible::Vec2>(path.at(0).position);

    Crucible::Vec2 guardDimensions{Crucible::TILE_SIZE, Crucible::TILE_SIZE};

    auto& transform = e.addComponent<Component::CTransform>(position, guardDimensions);

    std::shared_ptr<sf::VertexArray> vertices = std::make_shared<sf::VertexArray>(sf::Quads);
    vertices->append(sf::Vertex({transform.position->x, transform.position->y}));
    vertices->append(sf::Vertex({transform.position->x + guardDimensions.x, transform.position->y}));
    vertices->append(sf::Vertex({transform.position->x + guardDimensions.x, transform.position->y + guardDimensions.y}));
    vertices->append(sf::Vertex({transform.position->x, transform.position->y + Crucible::TILE_SIZE}));

    Tile guardTile(
            {static_cast<unsigned int>(position->x), static_cast<unsigned int>(position->y)},
            0,
            TileRotation::NONE,
            vertices);

    std::shared_ptr<sf::Texture>& texture = m_textureManager.getTexture(LevelManager::CATACOMB_MAIN_TILESET_PATH);

    std::vector<Crucible::Ray> rays = createRays(transform, lightingObjectLayerName);
    std::vector<std::vector<Crucible::LightRayIntersect>> defaultLightRayIntersects =
            std::vector<std::vector<Crucible::LightRayIntersect>>(rays.size(), std::vector<Crucible::LightRayIntersect>());
    e.addComponent<Component::CLightSource>(rays, sf::VertexArray(), defaultLightRayIntersects, lightingObjectLayerName);
    e.addComponent<Component::CTile>(guardTile, texture);
    e.addComponent<Component::CCollider>();
    e.addComponent<Component::CAnimation>(LevelManager::CATACOMB_MAIN_TILESET_PATH);

}

void EntitySpawner::createTile(Tile& t, bool isCollidable, bool immovable)
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::TILE);
    Crucible::Vec2 position{static_cast<float>(t.position.x), static_cast<float>(t.position.y)};

    Crucible::Vec2 tileDimensions{Crucible::TILE_SIZE, Crucible::TILE_SIZE};

    e.addComponent<Component::CTransform>(std::make_shared<Crucible::Vec2>(position), tileDimensions);
    if (isCollidable)
    {
        e.addComponent<Component::CCollider>(immovable);
    }

    sf::VertexArray vertices(sf::Quads);
    vertices.append(sf::Vertex({position.x, position.y}));
    vertices.append(sf::Vertex({position.x + tileDimensions.x, position.y}));
    vertices.append(sf::Vertex({position.x + tileDimensions.x, position.y + tileDimensions.y}));
    vertices.append(sf::Vertex({position.x, position.y + tileDimensions.y}));

    std::shared_ptr<sf::Texture>& texture = m_textureManager.getTexture(LevelManager::CATACOMB_MAIN_TILESET_PATH);
    t.vertices = std::make_shared<sf::VertexArray>(vertices);

    e.addComponent<Component::CTile>(t, texture);
    e.addComponent<Component::CAnimation>(LevelManager::CATACOMB_MAIN_TILESET_PATH);
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