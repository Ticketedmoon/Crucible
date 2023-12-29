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
    e.addComponent<Component::CShape>(shapeVertices);
    e.addComponent<Component::CLightSource>(rays, sf::VertexArray(), defaultLightRayIntersects);
}

void EntitySpawner::spawnWall(Crucible::Vec2 position, Crucible::Vec2 dimensions)
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::WALL);

    sf::VertexArray vertices(sf::Quads, 5);
    const sf::Color shapeColor = sf::Color::Blue;
    vertices[0] = sf::Vertex({position.x - dimensions.x / 2, position.y - dimensions.y / 2}, shapeColor);
    vertices[1] = sf::Vertex({position.x + dimensions.x / 2, position.y - dimensions.y / 2}, shapeColor);
    vertices[2] = sf::Vertex({position.x + dimensions.x / 2, position.y + dimensions.y / 2}, shapeColor);
    vertices[3] = sf::Vertex({position.x - dimensions.x / 2, position.y + dimensions.y / 2}, shapeColor);
    vertices[4] = sf::Vertex({position.x - dimensions.x / 2, position.y - dimensions.y / 2}, shapeColor);

    e.addComponent<Component::CTransform>(std::make_shared<Crucible::Vec2>(position));
    e.addComponent<Component::CShape>(vertices);
}

std::vector<Crucible::Ray> EntitySpawner::createRays(Component::CTransform& playerTransform)
{
    std::vector<Crucible::Ray> rays = std::vector<Crucible::Ray>();

    // Add wall vertices
    std::vector<Entity> wallEntities = m_entityManager.getEntitiesByComponentType<Component::CShape>();

    for (auto wallEntity : wallEntities)
    {
        Component::CShape s = wallEntity.getComponent<Component::CShape>();
        size_t shapeVertCount = s.vertices.getVertexCount();
        for (size_t vertIndex = 0; vertIndex < shapeVertCount; vertIndex++)
        {
            // @refactor: use move constructor here?
            sf::Vertex v = s.vertices[vertIndex];
            rays.emplace_back(playerTransform.position, Crucible::Vec2(v.position.x, v.position.y));
        }
    }

    // Add window corner vertices
    rays.emplace_back(playerTransform.position, Crucible::Vec2(0, 0));
    rays.emplace_back(playerTransform.position, Crucible::Vec2(Crucible::WINDOW_WIDTH, 0));
    rays.emplace_back(playerTransform.position, Crucible::Vec2(Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT));
    rays.emplace_back(playerTransform.position, Crucible::Vec2(0, Crucible::WINDOW_HEIGHT));

    std::cout << "Configured: [" << rays.size() << "] core light rays" << '\n';
    return rays;
}