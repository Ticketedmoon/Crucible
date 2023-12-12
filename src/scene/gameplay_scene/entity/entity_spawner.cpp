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

    std::vector<Crucible::Vertex> rayStartVertices;
    std::vector<Crucible::Vertex> rayEndVertices;

    int TOTAL_RAYS = 360;
    for (int i = 0; i < TOTAL_RAYS; i++)
    {
        rayStartVertices.emplace_back(Crucible::Vertex({position.x, position.y}, {0, 0}, sf::Color::Yellow));
        double r = dimensions.x / 2;
        constexpr double SCALE = 500.0f;
        float x = position.x + (cos(i) * r * SCALE);
        float y = position.y + (sin(i) * r * SCALE);
        Vec2 xy = Vec2(x, y);
        rayEndVertices.emplace_back(Crucible::Vertex({position.x, position.y}, xy, sf::Color::Yellow));
    }

    std::cout << "Found: [" << rayEndVertices.size() << "] light rays" << '\n';

    sf::VertexArray shapeVertices(sf::Quads);
    shapeVertices.append(sf::Vertex({position.x - dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Green));
    shapeVertices.append(sf::Vertex({position.x + dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Green));
    shapeVertices.append(sf::Vertex({position.x + dimensions.x/2, position.y + dimensions.y/2}, sf::Color::Green));
    shapeVertices.append(sf::Vertex({position.x - dimensions.x/2, position.y + dimensions.y/2}, sf::Color::Green));
    shapeVertices.append(sf::Vertex({position.x - dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Green));

    // FIXME temp magic num
    std::vector<std::vector<Crucible::LightRayIntersect>> defaultLightRayIntersects =
            std::vector<std::vector<Crucible::LightRayIntersect>>(TOTAL_RAYS, std::vector<Crucible::LightRayIntersect>());

    e.addComponent<Component::CControllable>();
    e.addComponent<Component::CTransform>(position);
    e.addComponent<Component::CShape>(shapeVertices);
    e.addComponent<Component::CLightSource>(rayStartVertices, rayEndVertices, sf::VertexArray(), defaultLightRayIntersects);
}

void EntitySpawner::spawnWall(Vec2 position, Vec2 dimensions)
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::WALL);

    sf::VertexArray vertices(sf::Quads, 5);
    vertices[0] = sf::Vertex({position.x - dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Blue);
    vertices[1] = sf::Vertex({position.x + dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Blue);
    vertices[2] = sf::Vertex({position.x + dimensions.x/2, position.y + dimensions.y/2}, sf::Color::Blue);
    vertices[3] = sf::Vertex({position.x - dimensions.x/2, position.y + dimensions.y/2}, sf::Color::Blue);
    vertices[4] = sf::Vertex({position.x - dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Blue);

    e.addComponent<Component::CTransform>(position);
    e.addComponent<Component::CShape>(vertices);
}