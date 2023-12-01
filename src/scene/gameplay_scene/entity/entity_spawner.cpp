#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void EntitySpawner::spawnPlayer()
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::PLAYER);

    Vec2 position{Crucible::WINDOW_WIDTH / 2.0f, Crucible::WINDOW_HEIGHT - 128};
    Vec2 dimensions{50, 50};

    sf::VertexArray vertices(sf::LineStrip, 5);
    vertices[0].position = sf::Vector2f(position.x - dimensions.x/2, position.y - dimensions.y/2);
    vertices[1].position = sf::Vector2f(position.x + dimensions.x/2, position.y - dimensions.y/2);
    vertices[2].position = sf::Vector2f(position.x + dimensions.x/2, position.y + dimensions.y/2);
    vertices[3].position = sf::Vector2f(position.x - dimensions.x/2, position.y + dimensions.y/2);
    vertices[4].position = sf::Vector2f(position.x - dimensions.x/2, position.y - dimensions.y/2);

    sf::VertexArray rayVertices(sf::LineStrip, 2);
    rayVertices[0].position = sf::Vector2f(position.x, position.y - dimensions.y / 2);
    rayVertices[1].position = sf::Vector2f(position.x, position.y - dimensions.y / 2);

    e.addComponent<Component::CControllable>();
    e.addComponent<Component::CTransform>(position);
    e.addComponent<Component::CShape>(vertices);
    e.addComponent<Component::CLightSource>(rayVertices);
}

void EntitySpawner::spawnWall(Vec2 position, Vec2 dimensions)
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::WALL);


    sf::VertexArray vertices(sf::LinesStrip, 5);
    vertices[0].position = sf::Vector2f(position.x - dimensions.x/2, position.y - dimensions.y/2);
    vertices[1].position = sf::Vector2f(position.x + dimensions.x/2, position.y - dimensions.y/2);
    vertices[2].position = sf::Vector2f(position.x + dimensions.x/2, position.y + dimensions.y/2);
    vertices[3].position = sf::Vector2f(position.x - dimensions.x/2, position.y + dimensions.y/2);
    vertices[4].position = sf::Vector2f(position.x - dimensions.x/2, position.y - dimensions.y/2);
    //shape.setFillColor(sf::Color::Blue);

    e.addComponent<Component::CTransform>(position);
    e.addComponent<Component::CShape>(vertices);
}