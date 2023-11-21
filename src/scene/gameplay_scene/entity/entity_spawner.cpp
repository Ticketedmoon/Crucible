#include "scene/gameplay_scene/entity/entity_spawner.h"

EntitySpawner::EntitySpawner(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void EntitySpawner::spawnPlayer()
{
    auto e = m_entityManager.addEntity(Entity::Type::PLAYER);

    e->addComponent<Component::CControllable>();

    Vec2 position{Crucible::WINDOW_WIDTH / 2, Crucible::WINDOW_HEIGHT / 2};
    e->addComponent<Component::CTransform>(position);

    Vec2 dimensions{50, 50};
    sf::RectangleShape shape({dimensions.x, dimensions.y});
    shape.setOrigin(dimensions.x / 2, dimensions.y / 2);
    shape.setPosition({position.x, position.y});

    e->addComponent<Component::CRectangleShape>(shape);
}

void EntitySpawner::spawnWall(Vec2 position, Vec2 dimensions)
{
    auto e = m_entityManager.addEntity(Entity::Type::WALL);

    e->addComponent<Component::CTransform>(position);

    sf::RectangleShape shape({dimensions.x, dimensions.y});
    shape.setOrigin(dimensions.x / 2, dimensions.y / 2);
    shape.setPosition({position.x, position.y});
    shape.setFillColor(sf::Color::Blue);

    e->addComponent<Component::CRectangleShape>(shape);
}