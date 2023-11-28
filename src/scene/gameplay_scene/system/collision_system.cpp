#include "collision_system.h"

CollisionSystem::CollisionSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void CollisionSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (const Entity entity : entities)
    {
        auto& entityRectangleShape = entity.getComponent<Component::CShape>();
        auto& entityTransform = entity.getComponent<Component::CTransform>();

        for (const Entity otherEntity : entities)
        {
            if (entity.getId() == otherEntity.getId())
            {
                // @Refactor: Can we do this a better way?
                continue;
            }
            auto& otherEntityRectangleShape = otherEntity.getComponent<Component::CShape>();
            auto& otherEntityTransform = otherEntity.getComponent<Component::CTransform>();

            sf::FloatRect overlap;
            if (CollisionSystem::isCollidingAABB(entityRectangleShape, otherEntityRectangleShape, overlap))
            {
                resolveCollision(entityRectangleShape, entityTransform, otherEntityRectangleShape, otherEntityTransform, overlap);
            }
        }
    }
}

bool CollisionSystem::isCollidingAABB(const Component::CShape& entityRect,
        const Component::CShape& otherEntityRect, sf::FloatRect& overlap)
{
    return entityRect.vertices.getBounds().intersects(otherEntityRect.vertices.getBounds(), overlap);
}

void CollisionSystem::resolveCollision(
        const Component::CShape& entityRectangleShape, Component::CTransform& entityTransform,
        const Component::CShape& otherEntityRectangleShape, const Component::CTransform& otherEntityTransform,
        const sf::FloatRect& overlap)
{
    float xDiff = std::abs(entityRectangleShape.vertices[0].position.x - entityRectangleShape.vertices[1].position.x) / 2;
    float yDiff = std::abs(entityRectangleShape.vertices[0].position.y - entityRectangleShape.vertices[2].position.y) / 2;

    float oxDiff = std::abs(otherEntityRectangleShape.vertices[0].position.x - otherEntityRectangleShape.vertices[1].position.x) / 2;
    float oyDiff = std::abs(otherEntityRectangleShape.vertices[0].position.y - otherEntityRectangleShape.vertices[2].position.y) / 2;

    const Vec2& entityPosition = Vec2(entityTransform.position.x - xDiff, entityTransform.position.y + yDiff);
    const Vec2& otherEntityPosition = Vec2(otherEntityTransform.position.x - oxDiff, otherEntityTransform.position.y + oyDiff);
    const Vec2& result = entityPosition - otherEntityPosition;

    applyCollisionManifoldToTransform(entityTransform, overlap, result);
}

void CollisionSystem::applyCollisionManifoldToTransform(Component::CTransform& entityTransform,
        const sf::FloatRect& overlap, const Vec2& result)
{
    sf::Vector2f collisionNormal{result.x, result.y};
    auto manifold = getManifold(overlap, collisionNormal);

    if (manifold.y == 1)
    {
        // Bottom Collision
        entityTransform.position.y -= overlap.height;
    }
    if (manifold.y == -1)
    {
        // Top Collision
        entityTransform.position.y += overlap.height;
    }

    if (manifold.x == 1)
    {
        // Left Collision
        entityTransform.position.x -= overlap.width;
    }
    if (manifold.x == -1)
    {
        // Right Collision
        entityTransform.position.x += overlap.width;
    }
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