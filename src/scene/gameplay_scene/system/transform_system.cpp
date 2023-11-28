#include <iostream>
#include "transform_system.h"

TransformSystem::TransformSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{
}

void TransformSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (const Entity entity : entities)
    {
        auto& entityTransform = entity.getComponent<Component::CTransform>();
        auto& entityRectangleShape = entity.getComponent<Component::CShape>();

        // @Refactor: Should we do this elsewhere?
        if (entity.hasComponent<Component::CControllable>())
        {
            resolveControllerMovementForEntity(entity, entityTransform);
        }

        float xDiff = std::abs(entityRectangleShape.vertices[0].position.x - entityRectangleShape.vertices[1].position.x) / 2;
        float yDiff = std::abs(entityRectangleShape.vertices[0].position.y - entityRectangleShape.vertices[2].position.y) / 2;

        // Update rect based on transform points
        entityRectangleShape.vertices[0].position = { entityTransform.position.x - xDiff, entityTransform.position.y - yDiff };
        entityRectangleShape.vertices[1].position = { entityTransform.position.x + xDiff, entityTransform.position.y - yDiff };
        entityRectangleShape.vertices[2].position = { entityTransform.position.x + xDiff, entityTransform.position.y + yDiff };
        entityRectangleShape.vertices[3].position = { entityTransform.position.x - xDiff, entityTransform.position.y + yDiff };
        entityRectangleShape.vertices[4].position = { entityTransform.position.x - xDiff, entityTransform.position.y - yDiff };
    }
}

void TransformSystem::resolveControllerMovementForEntity(const Entity& e, Component::CTransform& cTransform) const
{
    auto& controllable = e.getComponent<Component::CControllable>();
    float SPEED = 2.0f;

    if (controllable.isMovingLeft)
    {
        cTransform.position.x -= SPEED;
    }
    if (controllable.isMovingRight)
    {
        cTransform.position.x += SPEED;
    }
    if (controllable.isMovingUp)
    {
        cTransform.position.y -= SPEED;
    }
    if (controllable.isMovingDown)
    {
        cTransform.position.y += SPEED;
    }
}
