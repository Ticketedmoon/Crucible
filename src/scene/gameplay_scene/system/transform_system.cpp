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

        if (entity.hasComponent<Component::CLightSource>())
        {
            auto& lightSource = entity.getComponent<Component::CLightSource>();

            // TODO: We should just scale the vectors not arbitrarily update x or y.
            //       This will enable us to apply rotations on lines and arbitrarily scale until a collision.

            float xDiff = std::abs(entityRectangleShape.vertices[0].position.x - entityRectangleShape.vertices[1].position.x) / 2;
            float yDiff = std::abs(entityRectangleShape.vertices[0].position.y - entityRectangleShape.vertices[2].position.y) / 2;

            // up
            lightSource.rayVertices[0].position = {entityTransform.position.x, entityTransform.position.y - yDiff};
            lightSource.rayVertices[1].position.x = entityTransform.position.x;
            if ( lightSource.rayVertices[1].position.y > 0)
            {
                lightSource.rayVertices[1].position.y -= 100;
            }

            // down
            lightSource.rayVertices[2].position = {entityTransform.position.x, entityTransform.position.y + yDiff};
            lightSource.rayVertices[3].position.x = entityTransform.position.x;
            if ( lightSource.rayVertices[3].position.y < Crucible::WINDOW_HEIGHT)
            {
                lightSource.rayVertices[3].position.y += 100;
            }

            // left
            lightSource.rayVertices[4].position = {entityTransform.position.x - xDiff, entityTransform.position.y};
            lightSource.rayVertices[5].position.y = entityTransform.position.y;
            if ( lightSource.rayVertices[5].position.x > 0)
            {
                lightSource.rayVertices[5].position.x -= 100;
            }

            // right
            lightSource.rayVertices[6].position = {entityTransform.position.x + xDiff, entityTransform.position.y};
            lightSource.rayVertices[7].position.y = entityTransform.position.y;
            if ( lightSource.rayVertices[7].position.x < Crucible::WINDOW_WIDTH)
            {
                lightSource.rayVertices[7].position.x += 100;
            }
        }

        updateVertexPositionsForEntity(entityTransform, entityRectangleShape);
    }
}

void TransformSystem::resolveControllerMovementForEntity(const Entity& e, Component::CTransform& cTransform)
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

void TransformSystem::updateVertexPositionsForEntity(const Component::CTransform& entityTransform,
        Component::CShape& entityRectangleShape) const
{
    float xDiff = std::abs(entityRectangleShape.vertices[0].position.x - entityRectangleShape.vertices[1].position.x) / 2;
    float yDiff = std::abs(entityRectangleShape.vertices[0].position.y - entityRectangleShape.vertices[2].position.y) / 2;

    // Update rect based on transform points
    entityRectangleShape.vertices[0].position = { entityTransform.position.x - xDiff, entityTransform.position.y - yDiff };
    entityRectangleShape.vertices[1].position = { entityTransform.position.x + xDiff, entityTransform.position.y - yDiff };
    entityRectangleShape.vertices[2].position = { entityTransform.position.x + xDiff, entityTransform.position.y + yDiff };
    entityRectangleShape.vertices[3].position = { entityTransform.position.x - xDiff, entityTransform.position.y + yDiff };
    entityRectangleShape.vertices[4].position = { entityTransform.position.x - xDiff, entityTransform.position.y - yDiff };
}
