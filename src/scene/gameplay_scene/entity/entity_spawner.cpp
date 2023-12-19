#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void EntitySpawner::spawnPlayer()
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::PLAYER);

    Vec2 position{Crucible::WINDOW_WIDTH / 2.0f, Crucible::WINDOW_HEIGHT - 128};
    float PLAYER_SIZE = 50;
    Vec2 dimensions{PLAYER_SIZE, PLAYER_SIZE};

    std::vector<std::pair<Crucible::Vertex, Crucible::Vertex>> rays;

    constexpr int TOTAL_RAYS = 120;
    constexpr int TOTAL_DEGREES_CIRCLE = 360;
    assert(TOTAL_DEGREES_CIRCLE % TOTAL_RAYS == 0);

    constexpr int DEGREE_INCREMENT = TOTAL_DEGREES_CIRCLE / TOTAL_RAYS;
    constexpr int RAY_SPEED = 100;

    for (int rayAngleDegrees = 0; rayAngleDegrees <= TOTAL_DEGREES_CIRCLE; rayAngleDegrees += DEGREE_INCREMENT)
    {
        double rayAngleRadians = degrees_to_radians(rayAngleDegrees);
        double rayDirX = std::cos(rayAngleRadians) * RAY_SPEED;
        double rayDirY = std::sin(rayAngleRadians) * RAY_SPEED;
        Vec2 rayDirectionVector = Vec2(rayDirX, rayDirY);
        rays.emplace_back(
                Crucible::Vertex({position.x, position.y}, {0, 0}, sf::Color::Yellow),
                Crucible::Vertex({position.x, position.y}, rayDirectionVector, sf::Color::Yellow)
        );
    }

    std::cout << "Found: [" << rays.size() << "] light rays" << '\n';

    sf::VertexArray shapeVertices(sf::Quads);
    const sf::Color shapeColor = {255, 0, 127};
    shapeVertices.append(sf::Vertex({position.x - dimensions.x / 2, position.y - dimensions.y / 2}, shapeColor));
    shapeVertices.append(sf::Vertex({position.x + dimensions.x / 2, position.y - dimensions.y / 2}, shapeColor));
    shapeVertices.append(sf::Vertex({position.x + dimensions.x / 2, position.y + dimensions.y / 2}, shapeColor));
    shapeVertices.append(sf::Vertex({position.x - dimensions.x / 2, position.y + dimensions.y / 2}, shapeColor));
    shapeVertices.append(sf::Vertex({position.x - dimensions.x / 2, position.y - dimensions.y / 2}, shapeColor));

    std::vector<std::vector<Crucible::LightRayIntersect>> defaultLightRayIntersects =
            std::vector<std::vector<Crucible::LightRayIntersect>>(TOTAL_RAYS + 1,
                    std::vector<Crucible::LightRayIntersect>());

    e.addComponent<Component::CControllable>();
    e.addComponent<Component::CTransform>(position);
    e.addComponent<Component::CShape>(shapeVertices);
    e.addComponent<Component::CLightSource>(rays, sf::VertexArray(), defaultLightRayIntersects);
}

void EntitySpawner::spawnWall(Vec2 position, Vec2 dimensions)
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::WALL);

    sf::VertexArray vertices(sf::Quads, 5);
    const sf::Color shapeColor = sf::Color::Transparent;
    vertices[0] = sf::Vertex({position.x - dimensions.x / 2, position.y - dimensions.y / 2}, shapeColor);
    vertices[1] = sf::Vertex({position.x + dimensions.x / 2, position.y - dimensions.y / 2}, shapeColor);
    vertices[2] = sf::Vertex({position.x + dimensions.x / 2, position.y + dimensions.y / 2}, shapeColor);
    vertices[3] = sf::Vertex({position.x - dimensions.x / 2, position.y + dimensions.y / 2}, shapeColor);
    vertices[4] = sf::Vertex({position.x - dimensions.x / 2, position.y - dimensions.y / 2}, shapeColor);

    e.addComponent<Component::CTransform>(position);
    e.addComponent<Component::CShape>(vertices);
}

double EntitySpawner::degrees_to_radians(double y)
{
    double radians = (y * M_PI) / 180; // y not x
    return radians;
}