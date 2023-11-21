#include "collision_system.h"

CollisionSystem::CollisionSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void CollisionSystem::execute()
{
    std::vector<std::shared_ptr<Entity>> entities = m_entityManager.getEntities();
    for (const std::shared_ptr<Entity>& entity : entities)
    {
        auto& entityRectangleShape = entity->getComponent<Component::CRectangleShape>();
        for (const std::shared_ptr<Entity>& otherEntity : entities)
        {
            if (entity->getId() == otherEntity->getId())
            {
                continue;
            }
            auto& otherEntityRectangleShape = otherEntity->getComponent<Component::CRectangleShape>();

            sf::FloatRect overlap;
            if (CollisionSystem::isCollidingAABB(entityRectangleShape, otherEntityRectangleShape, overlap))
            {
                auto collisionNormal = entityRectangleShape.shape.getPosition() -
                        otherEntityRectangleShape.shape.getPosition();
                auto manifold = getManifold(overlap, collisionNormal);

                if (manifold.y == 1)
                {
                    // Bottom Collision
                    auto& entityTransform = entity->getComponent<Component::CTransform>();
                    entityTransform.position = {entityTransform.position.x, entityTransform.position.y-overlap.height};
                }

                if (manifold.y == -1)
                {
                    // Top Collision
                    auto& entityTransform = entity->getComponent<Component::CTransform>();
                    entityTransform.position = {entityTransform.position.x, entityTransform.position.y+overlap.height};
                }

                if (manifold.x == 1)
                {
                    // Left Collision
                    auto& entityTransform = entity->getComponent<Component::CTransform>();
                    entityTransform.position = {entityTransform.position.x-overlap.width, entityTransform.position.y};
                }
                else if (manifold.x == -1)
                {
                    // Right Collision
                    auto& entityTransform = entity->getComponent<Component::CTransform>();
                    entityTransform.position = {entityTransform.position.x+overlap.width, entityTransform.position.y};
                }
            }
        }
    }
}

bool CollisionSystem::isCollidingAABB(const Component::CRectangleShape& entityRect,
        const Component::CRectangleShape& otherEntityRect, sf::FloatRect& overlap)
{
    return entityRect.shape.getGlobalBounds().intersects(otherEntityRect.shape.getGlobalBounds(), overlap);
}

sf::Vector3f CollisionSystem::getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal)
{
    // the collision normal is stored in x and y, with the penetration in z
    sf::Vector3f manifold;

    if (overlap.width < overlap.height)
    {
        manifold.x = (collisionNormal.x < 0) ? 1.f : -1.f;
        manifold.z = overlap.width;
    }
    else
    {
        manifold.y = (collisionNormal.y < 0) ? 1.f : -1.f;
        manifold.z = overlap.height;
    }

    return manifold;
}