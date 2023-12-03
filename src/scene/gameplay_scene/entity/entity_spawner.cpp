#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void EntitySpawner::spawnPlayer()
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::PLAYER);

    Vec2 position{Crucible::WINDOW_WIDTH / 2.0f, Crucible::WINDOW_HEIGHT - 128};
    Vec2 dimensions{50, 50};

    sf::VertexArray vertices(sf::Quads);
    vertices.append(sf::Vertex({position.x - dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Green));
    vertices.append(sf::Vertex({position.x + dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Green));
    vertices.append(sf::Vertex({position.x + dimensions.x/2, position.y + dimensions.y/2}, sf::Color::Green));
    vertices.append(sf::Vertex({position.x - dimensions.x/2, position.y + dimensions.y/2}, sf::Color::Green));
    vertices.append(sf::Vertex({position.x - dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Green));

    sf::VertexArray rayVertices = sf::VertexArray(sf::LineStrip, 4);

    // Up
    rayVertices[0] = sf::Vertex({position.x, position.y - dimensions.y / 2}, sf::Color::Yellow);
    rayVertices[1] = sf::Vertex({position.x, position.y - dimensions.y / 2}, sf::Color::Yellow);

    // Left
    rayVertices[2] = sf::Vertex({position.x - dimensions.x / 2, position.y}, sf::Color::Yellow);
    rayVertices[3] = sf::Vertex({position.x - dimensions.x / 2, position.y}, sf::Color::Yellow);

    int TOTAL_LIGHT_RAYS = 2;
    assert(rayVertices.getVertexCount() == TOTAL_LIGHT_RAYS * 2);

    std::vector<std::vector<Crucible::LightRayIntersect>> defaultLightRayIntersects =
            std::vector<std::vector<Crucible::LightRayIntersect>>(TOTAL_LIGHT_RAYS, std::vector<Crucible::LightRayIntersect>());
    e.addComponent<Component::CControllable>();
    e.addComponent<Component::CTransform>(position);
    e.addComponent<Component::CShape>(vertices);
    e.addComponent<Component::CLightSource>(rayVertices, sf::VertexArray(), defaultLightRayIntersects);
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