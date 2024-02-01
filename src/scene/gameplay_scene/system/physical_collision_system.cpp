#include "physical_collision_system.h"

PhysicalCollisionSystem::PhysicalCollisionSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void PhysicalCollisionSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();

    for (const Entity& entity : entities)
    {
        auto& entityRectangleShape = entity.getComponent<Component::CTile>();
        auto& entityTransform = entity.getComponent<Component::CTransform>();
        auto& entityCollider = entity.getComponent<Component::CCollider>();

        for (const Entity& otherEntity : entities)
        {
            if (entity.getId() == otherEntity.getId())
            {
                // @Refactor: Can we do this a better way?
                continue;
            }

            if (entity.hasComponent<Component::CCollider>() && otherEntity.hasComponent<Component::CCollider>())
            {
                auto& otherEntityRectangleShape = otherEntity.getComponent<Component::CTile>();
                auto& otherEntityTransform = otherEntity.getComponent<Component::CTransform>();
                resolvePhysicalCollisions(entityRectangleShape, entityTransform, entityCollider,
                        otherEntityTransform, otherEntityRectangleShape);
            }
        }

        updateShapeVertexPositions(entityTransform, entityRectangleShape);
    }
}

void PhysicalCollisionSystem::resolvePhysicalCollisions(Component::CTile& entityRectangleShape,
        Component::CTransform& entityTransform, Component::CCollider entityCollider,
        Component::CTransform& otherEntityTransform, Component::CTile& otherEntityRectangleShape)
{
    if (entityCollider.immovable)
    {
        return;
    }

    sf::FloatRect overlap;
    if (isCollidingAABB(entityRectangleShape, otherEntityRectangleShape, overlap))
    {
        resolveCollision(entityRectangleShape, entityTransform, otherEntityRectangleShape, otherEntityTransform,
                overlap);
    }
}

bool PhysicalCollisionSystem::isCollidingAABB(const Component::CTile& entityTile,
        const Component::CTile& otherTile, sf::FloatRect& overlap)
{
    return entityTile.tile.vertices->getBounds().intersects(otherTile.tile.vertices->getBounds(), overlap);
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
        Component::CTile& entityTile)
{
    // Update rect based on transform points
    sf::VertexArray& tileVertices = *entityTile.tile.vertices;
    tileVertices[0].position = {entityTransform.position->x, entityTransform.position->y};
    tileVertices[1].position = {entityTransform.position->x + Crucible::TILE_SIZE, entityTransform.position->y};
    tileVertices[2].position = {entityTransform.position->x + Crucible::TILE_SIZE, entityTransform.position->y + Crucible::TILE_SIZE };
    tileVertices[3].position = {entityTransform.position->x, entityTransform.position->y + Crucible::TILE_SIZE };
    tileVertices[4].position = {entityTransform.position->x, entityTransform.position->y};
}

void PhysicalCollisionSystem::resolveCollision(Component::CTile& entityTile, Component::CTransform& entityTransform,
        Component::CTile& otherEntityTile, Component::CTransform& otherEntityTransform, const sf::FloatRect& overlap)
{
    sf::VertexArray& tileVertices = *entityTile.tile.vertices;
    float xDiff = std::abs(tileVertices[0].position.x - tileVertices[1].position.x) / 2;
    float yDiff = std::abs(tileVertices[0].position.y - tileVertices[2].position.y) / 2;

    float oxDiff = std::abs(tileVertices[0].position.x - tileVertices[1].position.x) / 2;
    float oyDiff = std::abs(tileVertices[0].position.y - tileVertices[2].position.y) / 2;

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