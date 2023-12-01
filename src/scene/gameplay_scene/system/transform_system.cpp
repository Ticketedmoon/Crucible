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

        bool isMoving;

        // @Refactor: Should we do this elsewhere?
        if (entity.hasComponent<Component::CControllable>())
        {
            isMoving = resolveControllerMovementForEntity(entity, entityTransform);
        }

        if (entity.hasComponent<Component::CLightSource>())
        {
            auto& lightSource = entity.getComponent<Component::CLightSource>();

            if (isMoving)
            {
                lightSource.lightVertices.clear();
            }

            float yDiff = std::abs(entityRectangleShape.vertices[0].position.y - entityRectangleShape.vertices[2].position.y) / 2;
            lightSource.rayVertices[0].position = {entityTransform.position.x, entityTransform.position.y - yDiff};

            sf::Vector2f& lightSourceRayEndPosition = lightSource.rayVertices[1].position;

            lightSourceRayEndPosition.x = entityTransform.position.x;
            if (lightSourceRayEndPosition.y > 0)
            {
                lightSourceRayEndPosition.y -= 10;
            }
        }

        updateVertexPositionsForEntity(entityTransform, entityRectangleShape);
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

bool TransformSystem::resolveControllerMovementForEntity(const Entity& e, Component::CTransform& cTransform)
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

    return controllable.isMovingLeft || controllable.isMovingRight || controllable.isMovingUp || controllable.isMovingDown;
}
