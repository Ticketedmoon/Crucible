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

            if (entity.hasComponent<Component::CLightSource>())
            {
                // @Refactor: Clean all of this up.
                auto& lightSource = entity.getComponent<Component::CLightSource>();
                Vec2 lightSourceRayStartPos = {lightSource.rayVertices[0].position.x, lightSource.rayVertices[0].position.y};
                Vec2 lightSourceRayEndPos = {lightSource.rayVertices[1].position.x, lightSource.rayVertices[1].position.y};

                Vec2 shapeLineStartPosA = {otherEntityRectangleShape.vertices[0].position.x, otherEntityRectangleShape.vertices[0].position.y};
                Vec2 shapeLineEndPosA = {otherEntityRectangleShape.vertices[1].position.x, otherEntityRectangleShape.vertices[1].position.y};

                Vec2 shapeLineStartPosB = {otherEntityRectangleShape.vertices[1].position.x, otherEntityRectangleShape.vertices[1].position.y};
                Vec2 shapeLineEndPosB = {otherEntityRectangleShape.vertices[2].position.x, otherEntityRectangleShape.vertices[2].position.y};

                Vec2 shapeLineStartPosC = {otherEntityRectangleShape.vertices[2].position.x, otherEntityRectangleShape.vertices[2].position.y};
                Vec2 shapeLineEndPosC = {otherEntityRectangleShape.vertices[3].position.x, otherEntityRectangleShape.vertices[3].position.y};

                Vec2 shapeLineStartPosD = {otherEntityRectangleShape.vertices[3].position.x, otherEntityRectangleShape.vertices[3].position.y};
                Vec2 shapeLineEndPosD = {otherEntityRectangleShape.vertices[4].position.x, otherEntityRectangleShape.vertices[4].position.y};

                sf::Color otherShapeColour = sf::Color::White;

                Crucible::LightRayIntersect lineIntersectA = isLineIntersecting(lightSourceRayStartPos, lightSourceRayEndPos, shapeLineStartPosA, shapeLineEndPosA);
                Crucible::LightRayIntersect lineIntersectB = isLineIntersecting(lightSourceRayStartPos, lightSourceRayEndPos, shapeLineStartPosB, shapeLineEndPosB);
                Crucible::LightRayIntersect lineIntersectC = isLineIntersecting(lightSourceRayStartPos, lightSourceRayEndPos, shapeLineStartPosC, shapeLineEndPosC);
                Crucible::LightRayIntersect lineIntersectD = isLineIntersecting(lightSourceRayStartPos, lightSourceRayEndPos, shapeLineStartPosD, shapeLineEndPosD);

                if (lineIntersectD.result)
                {
                    otherShapeColour = sf::Color::Green;
                    lightSource.lightRayIntersects.emplace_back(lineIntersectD);
                }
                else if (lineIntersectC.result)
                {
                    otherShapeColour = sf::Color::Green;
                    lightSource.lightRayIntersects.emplace_back(lineIntersectC);
                }
                else if (lineIntersectB.result)
                {
                    otherShapeColour = sf::Color::Green;
                    lightSource.lightRayIntersects.emplace_back(lineIntersectB);
                }
                else if (lineIntersectA.result)
                {
                    otherShapeColour = sf::Color::Green;
                    lightSource.lightRayIntersects.emplace_back(lineIntersectA);
                }
            }

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

Crucible::LightRayIntersect CollisionSystem::isLineIntersecting(Vec2 vertexA, Vec2 vertexB, Vec2 vertexC, Vec2 vertexD)
{
    Vec2 r = (vertexB - vertexA);
    Vec2 s = (vertexD - vertexC);
    Vec2 cma = vertexC - vertexA;

    float rCrossS = crossProduct(r, s);
    float cmaCrossS = crossProduct(cma, s);
    float cmaCrossR = crossProduct(cma, r);

    float t = (cmaCrossS / rCrossS);
    float u = (cmaCrossR / rCrossS);

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        float collisionVertexX = vertexA.x + (t * r.x);
        float collisionVertexY = vertexA.y + (t * r.y);
        return {true, Vec2(collisionVertexX, collisionVertexY)};
    }

    return {false, Vec2(0, 0)};
}

float CollisionSystem::crossProduct(Vec2 a, Vec2 b)
{
    return a.x * b.y - b.x * a.y;;
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