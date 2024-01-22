#include "physical_collision_system.h"

PhysicalCollisionSystem::PhysicalCollisionSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void PhysicalCollisionSystem::execute()
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

            resolvePhysicalCollisions(entityRectangleShape, entityTransform, otherEntityTransform,
                    otherEntityRectangleShape);
        }

        updateShapeVertexPositions(entityTransform, entityRectangleShape);
    }
}

void PhysicalCollisionSystem::resolvePhysicalCollisions(Component::CShape& entityRectangleShape,
        Component::CTransform& entityTransform, Component::CTransform& otherEntityTransform,
        Component::CShape& otherEntityRectangleShape)
{
    sf::FloatRect overlap;
    if (isCollidingAABB(entityRectangleShape, otherEntityRectangleShape, overlap))
    {
        resolveCollision(entityRectangleShape, entityTransform, otherEntityRectangleShape, otherEntityTransform,
                overlap);
    }
}

bool PhysicalCollisionSystem::isCollidingAABB(const Component::CShape& entityRect,
        const Component::CShape& otherEntityRect, sf::FloatRect& overlap)
{
    return entityRect.vertices.getBounds().intersects(otherEntityRect.vertices.getBounds(), overlap);
}

void PhysicalCollisionSystem::applyCollisionOverlapToEntityTransform(Component::CTransform& entityTransform,
        sf::Vector3f manifoldDist, sf::Vector2f collisionOverlap)
{
    if (manifoldDist.y == 1)
    {
        // Bottom Collision
        entityTransform.position->y -= collisionOverlap.y;
    }
    if (manifoldDist.y == -1)
    {
        // Top Collision
        entityTransform.position->y += collisionOverlap.y;
    }

    if (manifoldDist.x == 1)
    {
        // Left Collision
        entityTransform.position->x -= collisionOverlap.x;
    }
    if (manifoldDist.x == -1)
    {
        // Right Collision
        entityTransform.position->x += collisionOverlap.x;
    }
}

void PhysicalCollisionSystem::updateShapeVertexPositions(const Component::CTransform& entityTransform,
        Component::CShape& entityRectangleShape)
{
    float xDiff = std::abs(entityRectangleShape.vertices[0].position.x - entityRectangleShape.vertices[1].position.x) / 2;
    float yDiff = std::abs(entityRectangleShape.vertices[0].position.y - entityRectangleShape.vertices[2].position.y) / 2;

    // Update rect based on transform points
    entityRectangleShape.vertices[0].position = { entityTransform.position->x - xDiff, entityTransform.position->y - yDiff };
    entityRectangleShape.vertices[1].position = { entityTransform.position->x + xDiff, entityTransform.position->y - yDiff };
    entityRectangleShape.vertices[2].position = { entityTransform.position->x + xDiff, entityTransform.position->y + yDiff };
    entityRectangleShape.vertices[3].position = { entityTransform.position->x - xDiff, entityTransform.position->y + yDiff };
    entityRectangleShape.vertices[4].position = { entityTransform.position->x - xDiff, entityTransform.position->y - yDiff };
}

void PhysicalCollisionSystem::resolveCollision(Component::CShape& entityRectangleShape, Component::CTransform& entityTransform,
        Component::CShape& otherEntityRectangleShape, Component::CTransform& otherEntityTransform,
        const sf::FloatRect& overlap)
{
    float xDiff = std::abs(entityRectangleShape.vertices[0].position.x - entityRectangleShape.vertices[1].position.x) / 2;
    float yDiff = std::abs(entityRectangleShape.vertices[0].position.y - entityRectangleShape.vertices[2].position.y) / 2;

    float oxDiff = std::abs(otherEntityRectangleShape.vertices[0].position.x - otherEntityRectangleShape.vertices[1].position.x) / 2;
    float oyDiff = std::abs(otherEntityRectangleShape.vertices[0].position.y - otherEntityRectangleShape.vertices[2].position.y) / 2;

    const Crucible::Vec2& entityPosition = Crucible::Vec2(entityTransform.position->x - xDiff, entityTransform.position->y + yDiff);
    const Crucible::Vec2& otherEntityPosition = Crucible::Vec2(otherEntityTransform.position->x - oxDiff, otherEntityTransform.position->y + oyDiff);
    const Crucible::Vec2& result = entityPosition - otherEntityPosition;

    applyCollisionManifoldToTransform(entityTransform, overlap, result);
}

void PhysicalCollisionSystem::applyCollisionManifoldToTransform(Component::CTransform& cTransform,
        const sf::FloatRect& overlap, const Crucible::Vec2& result)
{
    sf::Vector2f collisionNormal{result.x, result.y};
    sf::Vector3f manifoldDist = getManifold(overlap, collisionNormal);
    sf::Vector2f collisionOverlap = {overlap.width, overlap.height};

    applyCollisionOverlapToEntityTransform(cTransform, manifoldDist, collisionOverlap);
}

sf::Vector3f PhysicalCollisionSystem::getManifold(const sf::FloatRect& overlap, const sf::Vector2f& collisionNormal)
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