#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager) : m_entityManager(entityManager)
{
}

void EntitySpawner::spawnPlayer()
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::PLAYER);

    std::shared_ptr<Crucible::Vec2> position = std::make_shared<Crucible::Vec2>(Crucible::WINDOW_WIDTH / 2.0f,
            Crucible::WINDOW_HEIGHT - 128);
    Crucible::Vec2 dimensions{Crucible::PLAYER_SIZE, Crucible::PLAYER_SIZE};
    
    auto& playerTransform = e.addComponent<Component::CTransform>(position);
    
    std::vector<Crucible::Ray> rays = createRays(playerTransform);

    sf::VertexArray shapeVertices(sf::Quads);
    const sf::Color playerColor = {255, 0, 127};
    shapeVertices.append(sf::Vertex({position->x - dimensions.x / 2, position->y - dimensions.y / 2}, playerColor));
    shapeVertices.append(sf::Vertex({position->x + dimensions.x / 2, position->y - dimensions.y / 2}, playerColor));
    shapeVertices.append(sf::Vertex({position->x + dimensions.x / 2, position->y + dimensions.y / 2}, playerColor));
    shapeVertices.append(sf::Vertex({position->x - dimensions.x / 2, position->y + dimensions.y / 2}, playerColor));
    shapeVertices.append(sf::Vertex({position->x - dimensions.x / 2, position->y - dimensions.y / 2}, playerColor));

    std::vector<std::vector<Crucible::LightRayIntersect>> defaultLightRayIntersects =
            std::vector<std::vector<Crucible::LightRayIntersect>>(rays.size(), std::vector<Crucible::LightRayIntersect>());

    e.addComponent<Component::CControllable>();
    e.addComponent<Component::CCollider>();
    e.addComponent<Component::CShape>(shapeVertices);
    e.addComponent<Component::CLightSource>(rays, sf::VertexArray(), defaultLightRayIntersects);
}

void EntitySpawner::spawnWall(Crucible::Vec2 position, Crucible::Vec2 dimensions, bool isCollidable, sf::Color wallColor)
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::WALL);

    sf::VertexArray vertices(sf::Quads, 5);
    const sf::Color shapeColor = wallColor;
    float halfEntityWidth = dimensions.x / 2;
    float halfEntityHeight = dimensions.y / 2;
    vertices[0] = sf::Vertex({position.x - halfEntityWidth, position.y - halfEntityHeight}, shapeColor);
    vertices[1] = sf::Vertex({position.x + halfEntityWidth, position.y - halfEntityHeight}, shapeColor);
    vertices[2] = sf::Vertex({position.x + halfEntityWidth, position.y + halfEntityHeight}, shapeColor);
    vertices[3] = sf::Vertex({position.x - halfEntityWidth, position.y + halfEntityHeight}, shapeColor);
    vertices[4] = sf::Vertex({position.x - halfEntityWidth, position.y - halfEntityHeight}, shapeColor);

    e.addComponent<Component::CTransform>(std::make_shared<Crucible::Vec2>(position));
    e.addComponent<Component::CShape>(vertices);
    if (isCollidable)
    {
        e.addComponent<Component::CCollider>();
    }
}

std::vector<Crucible::Ray> EntitySpawner::createRays(Component::CTransform& playerTransform)
{
    std::vector<Crucible::Ray> rays = std::vector<Crucible::Ray>();
    std::vector<Crucible::Ray> additionalRays = std::vector<Crucible::Ray>();

    // Add window corner vertex rays
    rays.emplace_back(playerTransform.position, Crucible::Vec2(0, 0));
    rays.emplace_back(playerTransform.position, Crucible::Vec2(Crucible::WINDOW_WIDTH, 0));
    rays.emplace_back(playerTransform.position, Crucible::Vec2(0, Crucible::WINDOW_HEIGHT));
    rays.emplace_back(playerTransform.position, Crucible::Vec2(Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT));

    // Add wall vertices rays
    std::vector<Entity> wallEntities = m_entityManager.getEntitiesByComponentType<Component::CShape>();
    for (auto wallEntity : wallEntities)
    {
        Component::CShape s = wallEntity.getComponent<Component::CShape>();
        for (size_t vertIndex = 0; vertIndex < s.vertices.getVertexCount()-1; vertIndex++)
        {
            sf::Vertex v = s.vertices[vertIndex];

            // Add core ray
            rays.emplace_back(playerTransform.position, Crucible::Vec2(v.position.x, v.position.y));

            // Add additional rays to left and right of core ray (This happens in RayAppenderSystem)
            additionalRays.emplace_back(playerTransform.position, Crucible::Vec2(0, 0));
            additionalRays.emplace_back(playerTransform.position, Crucible::Vec2(0, 0));
            additionalRays.emplace_back(playerTransform.position, Crucible::Vec2(0, 0));
            additionalRays.emplace_back(playerTransform.position, Crucible::Vec2(0, 0));
        }
    }

    rays.insert(rays.end(), additionalRays.begin(), additionalRays.end());

    std::cout << "Configured: [" << rays.size() << "] light rays" << '\n';

    return rays;
}