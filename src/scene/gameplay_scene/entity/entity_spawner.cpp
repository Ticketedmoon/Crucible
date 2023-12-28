#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager) : m_entityManager(entityManager)
{
    assert(Crucible::TOTAL_DEGREES_CIRCLE % Crucible::TOTAL_CORE_LIGHT_RAYS == 0);
}

void EntitySpawner::spawnPlayer()
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::PLAYER);

    Crucible::Vec2 position{Crucible::WINDOW_WIDTH / 2.0f, Crucible::WINDOW_HEIGHT - 128};
    Crucible::Vec2 dimensions{Crucible::PLAYER_SIZE, Crucible::PLAYER_SIZE};
    
    Component::CTransform playerTransform = e.addComponent<Component::CTransform>(position);
    
    std::vector<Crucible::Ray> rays = createRays(playerTransform);

    sf::VertexArray shapeVertices(sf::Quads);
    const sf::Color playerColor = {255, 0, 127};
    shapeVertices.append(sf::Vertex({position.x - dimensions.x / 2, position.y - dimensions.y / 2}, playerColor));
    shapeVertices.append(sf::Vertex({position.x + dimensions.x / 2, position.y - dimensions.y / 2}, playerColor));
    shapeVertices.append(sf::Vertex({position.x + dimensions.x / 2, position.y + dimensions.y / 2}, playerColor));
    shapeVertices.append(sf::Vertex({position.x - dimensions.x / 2, position.y + dimensions.y / 2}, playerColor));
    shapeVertices.append(sf::Vertex({position.x - dimensions.x / 2, position.y - dimensions.y / 2}, playerColor));

    std::vector<std::vector<Crucible::LightRayIntersect>> defaultLightRayIntersects =
            std::vector<std::vector<Crucible::LightRayIntersect>>(Crucible::TOTAL_CORE_LIGHT_RAYS + Crucible::TOTAL_ADDITIONAL_CORNER_RAYS + 1,
                    std::vector<Crucible::LightRayIntersect>());

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

    e.addComponent<Component::CTransform>(position);
    e.addComponent<Component::CShape>(vertices);
}

std::vector<Crucible::Ray> EntitySpawner::createRays(Component::CTransform& playerTransform)
{
    std::vector<Crucible::Ray> rays = std::vector<Crucible::Ray>(Crucible::TOTAL_RAYS + 1);

    for (size_t rayIndex = 0; rayIndex <= Crucible::TOTAL_CORE_LIGHT_RAYS; rayIndex++)
    {
        const double rayAngleRadians = degrees_to_radians(Crucible::DEGREE_INCREMENT * rayIndex);
        const double rayDirX = cos(rayAngleRadians) * Crucible::RAY_SPEED;
        const double rayDirY = sin(rayAngleRadians) * Crucible::RAY_SPEED;
        // @refactor: use move constructor here?
        rays[rayIndex] = Crucible::Ray(playerTransform.position,  Crucible::Vec2(rayDirX, rayDirY));
    }

    std::cout << "Configured: [" << rays.size() << "] core light rays" << '\n';
    return rays;
}

double EntitySpawner::degrees_to_radians(double y)
{
    double radians = (y * M_PI) / 180; // y not x
    return radians;
}