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

            // up
            // in entity_spawner find vertex for vertex[1], so it points in directly.
            // This system should only scale the vector until it reaches interesectionds);

            // scale
            for (Crucible::Vertex& v : lightSource.rayStartVertices)
            {
                // Start rays to scale based off player position
                v.position = {entityTransform.position.x, entityTransform.position.y};
            }
            for (Crucible::Vertex& v : lightSource.rayEndVertices)
            {
                // end points can scale normally
                v.position += v.increment;
            }
        }

        updateVertexPositionsForEntity(entityTransform, entityRectangleShape);
    }
}

void TransformSystem::resolveControllerMovementForEntity(const Entity& e, Component::CTransform& cTransform)
{
    auto& controllable = e.getComponent<Component::CControllable>();
    float SPEED = 3.5f;

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
