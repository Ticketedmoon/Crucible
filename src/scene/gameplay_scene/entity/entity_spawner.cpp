#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void EntitySpawner::spawnPlayer()
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::PLAYER);

    Vec2 position{Crucible::WINDOW_WIDTH / 2.0f, Crucible::WINDOW_HEIGHT - 128};
    Vec2 dimensions{50, 50};

    sf::VertexArray vertices(sf::Quads, 5);
    vertices[0] = sf::Vertex({position.x - dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Green);
    vertices[1] = sf::Vertex({position.x + dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Green);
    vertices[2] = sf::Vertex({position.x + dimensions.x/2, position.y + dimensions.y/2}, sf::Color::Green);
    vertices[3] = sf::Vertex({position.x - dimensions.x/2, position.y + dimensions.y/2}, sf::Color::Green);
    vertices[4] = sf::Vertex({position.x - dimensions.x/2, position.y - dimensions.y/2}, sf::Color::Green);

    sf::VertexArray rayVertices(sf::LineStrip, 2);
    rayVertices[0] = sf::Vertex({position.x, position.y - dimensions.y / 2}, sf::Color::Yellow);
    rayVertices[1] = sf::Vertex({position.x, position.y - dimensions.y / 2}, sf::Color::Yellow);

    e.addComponent<Component::CControllable>();
    e.addComponent<Component::CTransform>(position);
    e.addComponent<Component::CShape>(vertices);
    e.addComponent<Component::CLightSource>(rayVertices);
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