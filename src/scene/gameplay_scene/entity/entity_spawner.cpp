#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void EntitySpawner::spawnPlayer()
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::PLAYER);

    e.addComponent<Component::CControllable>();

    Vec2 position{Crucible::WINDOW_WIDTH / 2, Crucible::WINDOW_HEIGHT - 128};
    e.addComponent<Component::CTransform>(position);

    Vec2 dimensions{50, 50};

    sf::VertexArray points(sf::LinesStrip, 5);
    points[0].position = sf::Vector2f(position.x - dimensions.x/2, position.y - dimensions.y/2);
    points[1].position = sf::Vector2f(position.x + dimensions.x/2, position.y - dimensions.y/2);
    points[2].position = sf::Vector2f(position.x + dimensions.x/2, position.y + dimensions.y/2);
    points[3].position = sf::Vector2f(position.x - dimensions.x/2, position.y + dimensions.y/2);
    points[4].position = sf::Vector2f(position.x - dimensions.x/2, position.y - dimensions.y/2);

    e.addComponent<Component::CShape>(points);
}

void EntitySpawner::spawnWall(Vec2 position, Vec2 dimensions)
{
    auto e = m_entityManager.addEntity(Crucible::EntityType::WALL);

    e.addComponent<Component::CTransform>(position);

    sf::VertexArray points(sf::LinesStrip, 5);
    points[0].position = sf::Vector2f(position.x - dimensions.x/2, position.y - dimensions.y/2);
    points[1].position = sf::Vector2f(position.x + dimensions.x/2, position.y - dimensions.y/2);
    points[2].position = sf::Vector2f(position.x + dimensions.x/2, position.y + dimensions.y/2);
    points[3].position = sf::Vector2f(position.x - dimensions.x/2, position.y + dimensions.y/2);
    points[4].position = sf::Vector2f(position.x - dimensions.x/2, position.y - dimensions.y/2);
    //shape.setFillColor(sf::Color::Blue);

    e.addComponent<Component::CShape>(points);
}