#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager, TextureManager& textureManager)
    : m_entityManager(entityManager), m_textureManager(textureManager)
{
}

void EntitySpawner::createPlayer()
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::PLAYER);

    Crucible::Vec2 playerDimensions{Crucible::TILE_SIZE, Crucible::TILE_SIZE * 2};
    std::shared_ptr<Crucible::Vec2> position = std::make_shared<Crucible::Vec2>(Crucible::PLAYER_SPAWN_LOCATIONS[0]);

    auto& playerTransform = e.addComponent<Component::CTransform>(position, playerDimensions);

    std::shared_ptr<sf::VertexArray> vertices = std::make_shared<sf::VertexArray>(sf::Quads);
    vertices->append(sf::Vertex({playerTransform.position->x, playerTransform.position->y}));
    vertices->append(sf::Vertex({playerTransform.position->x + playerDimensions.x, playerTransform.position->y}));
    vertices->append(sf::Vertex({playerTransform.position->x + playerDimensions.x, playerTransform.position->y + playerDimensions.y}));
    vertices->append(sf::Vertex({playerTransform.position->x, playerTransform.position->y + playerDimensions.y}));

    Tile playerTile(
            {static_cast<unsigned int>(position->x), static_cast<unsigned int>(position->y)},
            vertices);

    std::shared_ptr<sf::Texture>& texture = m_textureManager.getTexture(LevelManager::PLAYER_SPRITE_SHEET_PATH);

    std::unordered_map<EntityAnimation, Component::AnimationGroup> animations{
            {EntityAnimation::PLAYER_WALK_DOWN,  Component::AnimationGroup({0, 1, 2, 3, 4, 5})},
            {EntityAnimation::PLAYER_WALK_UP,    Component::AnimationGroup({6, 7, 8, 9, 10, 11})},
            {EntityAnimation::PLAYER_WALK_RIGHT, Component::AnimationGroup({12, 13, 14, 15, 16, 17})},
            {EntityAnimation::PLAYER_WALK_LEFT,  Component::AnimationGroup({18, 19, 20, 21, 22, 23})},
            {EntityAnimation::PLAYER_IDLE_DOWN,  Component::AnimationGroup({24, 25, 26, 27, 28, 29}, {0, 1.0f/1.25f})},
            {EntityAnimation::PLAYER_IDLE_UP,    Component::AnimationGroup({30, 31, 32, 33, 34, 35}, {0, 1.0f/0.65f})},
            {EntityAnimation::PLAYER_IDLE_RIGHT, Component::AnimationGroup({36, 37, 38, 39, 40, 41}, {0, 1.0f/0.8f})},
            {EntityAnimation::PLAYER_IDLE_LEFT,  Component::AnimationGroup({42, 43, 44, 45, 46, 47}, {0, 1.0f/0.8f})},
    };

    e.addComponent<Component::CAnimation>(LevelManager::PLAYER_SPRITE_SHEET_PATH, EntityDirection::DOWN,
            EntityAnimation::PLAYER_IDLE_DOWN, 0, animations);
    e.addComponent<Component::CControllable>();
    e.addComponent<Component::CCollider>();
    e.addComponent<Component::CTile>(playerTile, texture);
}

void EntitySpawner::createGuard(const std::string& lightingObjectLayerName, const std::string& pathingObjectLayerName)
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::GUARD);
    std::unordered_map<std::string, ObjectLayer>& pathingObjectLayerNameToObjectLayer
        = LevelManager::activeLevel.layerNameToObjectLayer;
    ObjectLayer pathingObjectLayer = pathingObjectLayerNameToObjectLayer.at(pathingObjectLayerName);

    std::vector<Waypoint> path;
    for (size_t i = 0; i < pathingObjectLayer.objectData.size(); i++)
    {
        size_t pointIndex = pathingObjectLayer.customProperties.contains("point_idx")
                ? std::stoi(pathingObjectLayer.customProperties.at("point_idx").at(0).value)
                : 0;

        const sf::VertexArray pathingVertices = *pathingObjectLayer.objectData[i].objectVertices;
        for (size_t j = 0; j < pathingVertices.getVertexCount(); j++)
        {
            uint32_t waitPeriodMs = pathingObjectLayer.customProperties.contains("wait_period") && pointIndex == j
                    ? std::stoi(pathingObjectLayer.customProperties.at("wait_period").at(0).value)
                    : 0;
            path.emplace_back(Waypoint({pathingVertices[j].position.x, pathingVertices[j].position.y}, waitPeriodMs));
        }
    }

    e.addComponent<Component::CPathFollower>(path, pathingObjectLayerName);

    std::shared_ptr<Crucible::Vec2> position = std::make_shared<Crucible::Vec2>(path.at(0).position);

    Crucible::Vec2 guardDimensions{Crucible::TILE_SIZE, Crucible::TILE_SIZE * 2};

    auto& transform = e.addComponent<Component::CTransform>(position, guardDimensions);

    std::shared_ptr<sf::VertexArray> vertices = std::make_shared<sf::VertexArray>(sf::Quads);
    vertices->append(sf::Vertex({transform.position->x, transform.position->y}));
    vertices->append(sf::Vertex({transform.position->x + guardDimensions.x, transform.position->y}));
    vertices->append(sf::Vertex({transform.position->x + guardDimensions.x, transform.position->y + guardDimensions.y}));
    vertices->append(sf::Vertex({transform.position->x, transform.position->y + guardDimensions.y}));

    Tile guardTile(
            {static_cast<unsigned int>(position->x), static_cast<unsigned int>(position->y)},
            vertices);

    const std::string& mainTilesetPath = LevelManager::activeLevel.tileSets[0].path;
    std::shared_ptr<sf::Texture>& texture = m_textureManager.getTexture(mainTilesetPath);

    Component::CLightSource cLightSource = createLightSource(transform, lightingObjectLayerName);

    e.addComponent<Component::CLightSource>(cLightSource);
    e.addComponent<Component::CCollider>();

    // FIXME this is temporary, use different texture
    std::shared_ptr<sf::Texture>& p_texture = m_textureManager.getTexture(LevelManager::PLAYER_SPRITE_SHEET_PATH);
    e.addComponent<Component::CTile>(guardTile, p_texture);

    std::unordered_map<EntityAnimation, Component::AnimationGroup> animations{
            {EntityAnimation::PLAYER_IDLE_DOWN,  Component::AnimationGroup({24, 25, 26, 27, 28, 29}, {0, 1.0f/3.0f})},
            {EntityAnimation::PLAYER_WALK_DOWN,  Component::AnimationGroup({0, 1, 2, 3, 4, 5})},
            {EntityAnimation::PLAYER_WALK_UP,    Component::AnimationGroup({6, 7, 8, 9, 10, 11})},
            {EntityAnimation::PLAYER_WALK_RIGHT, Component::AnimationGroup({12, 13, 14, 15, 16, 17})},
            {EntityAnimation::PLAYER_WALK_LEFT,  Component::AnimationGroup({18, 19, 20, 21, 22, 23})}
    };;

    e.addComponent<Component::CAnimation>(LevelManager::PLAYER_SPRITE_SHEET_PATH, EntityDirection::DOWN,
            EntityAnimation::PLAYER_IDLE_DOWN, 0, animations);
}

Component::CLightSource EntitySpawner::createLightSource(Component::CTransform& playerTransform, const std::string& layerName)
{
    std::vector<Crucible::Ray> coreRays = std::vector<Crucible::Ray>();
    std::vector<Crucible::Ray> additionalRays = std::vector<Crucible::Ray>();

    ObjectLayer& objectLayer = LevelManager::activeLevel.layerNameToObjectLayer.at(layerName);

    for (const Object& tileObject : objectLayer.objectData)
    {
        std::shared_ptr<sf::VertexArray> tileObjectVertices = tileObject.objectVertices;
        for (size_t i = 0; i < tileObjectVertices->getVertexCount(); i++)
        {
            const sf::Vertex& v = (*tileObjectVertices)[i];
            // Add core ray
            coreRays.emplace_back(playerTransform.position, Crucible::Vec2(v.position.x, v.position.y));
            // Add additional rays to left and right of core ray (This happens in RayAppenderSystem)
            additionalRays.emplace_back(playerTransform.position, Crucible::Vec2(0, 0));
            additionalRays.emplace_back(playerTransform.position, Crucible::Vec2(0, 0));
        }
    }

    std::cout << "Configured: [" << coreRays.size() << "] core light rays" << '\n';
    std::cout << "Configured: [" << additionalRays.size() << "] additional light rays" << '\n';
    std::cout << "Configured: [" << coreRays.size() + additionalRays.size() << "] total light rays" << '\n';

    std::unordered_map<Crucible::RayType, Crucible::LightRayGroup> lightRayGroups{
        {Crucible::RayType::CORE, {coreRays, std::vector<std::vector<Crucible::LightRayIntersect>>(coreRays.size(), std::vector<Crucible::LightRayIntersect>())}},
        {Crucible::RayType::ADDITIONAL, {additionalRays, std::vector<std::vector<Crucible::LightRayIntersect>>(additionalRays.size(), std::vector<Crucible::LightRayIntersect>())}}
    };
    return {lightRayGroups, sf::VertexArray(), layerName};
}