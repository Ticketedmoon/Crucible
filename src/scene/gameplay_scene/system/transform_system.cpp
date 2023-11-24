#include "transform_system.h"

TransformSystem::TransformSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{
}

void TransformSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> entities = m_entityManager.getEntities();
    for (const std::shared_ptr<Entity>& e : entities)
    {
        auto& cTransform = e->getComponent<Component::CTransform>();
        if (e->hasComponent<Component::CControllable>())
        {
            auto& controllable = e->getComponent<Component::CControllable>();
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
    }
}