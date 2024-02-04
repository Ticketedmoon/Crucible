#include "transform_system.h"

TransformSystem::TransformSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{
}

void TransformSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (const Entity& entity : entities)
    {
        auto& entityTransform = entity.getComponent<Component::CTransform>();

        // @Refactor: Should we do this elsewhere?
        if (entity.hasComponent<Component::CControllable>())
        {
            resolveControllerMovementForEntity(entity, entityTransform);
        }

        if (entity.hasComponent<Component::CPathFollower>())
        {
            auto& cPathFollower = entity.getComponent<Component::CPathFollower>();

            // If destinationIndex is == path.size(), then reset to 0.
            if (cPathFollower.destinationIndex == cPathFollower.path.size())
            {
                cPathFollower.destinationIndex = 0;
            }

            // If at point, move to next destination
            if (*entityTransform.position == cPathFollower.path[cPathFollower.destinationIndex])
            {
                cPathFollower.destinationIndex++;
            }

            // Move towards destination point
            int sign = entityTransform.position->x < cPathFollower.path[cPathFollower.destinationIndex].x ? 1 : -1;
            entityTransform.position->x += sign;
            //entityTransform.position->y = 0;
        }
    }
}

void TransformSystem::resolveControllerMovementForEntity(const Entity& e, Component::CTransform& cTransform)
{
    auto& controllable = e.getComponent<Component::CControllable>();
    float SPEED = 2.5f;

    if (controllable.isMovingLeft)
    {
        cTransform.position->x -= SPEED;
    }
    if (controllable.isMovingRight)
    {
        cTransform.position->x += SPEED;
    }
    if (controllable.isMovingUp)
    {
        cTransform.position->y -= SPEED;
    }
    if (controllable.isMovingDown)
    {
        cTransform.position->y += SPEED;
    }
}