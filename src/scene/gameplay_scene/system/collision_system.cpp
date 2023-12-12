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

            if (entity.hasComponent<Component::CLightSource>())
            {
                auto& lightSource = entity.getComponent<Component::CLightSource>();

                for (int lineIndex = 0; lineIndex <  lightSource.rayStartVertices.size(); lineIndex++)
                {
                    // @Refactor: Rather than order these in reverse, sort by closest distance to line for a more scalable solution.
                    bool hasIntersectA = checkForLightIntersectWithShape(lightSource, otherEntityRectangleShape, lineIndex, 3, 4);
                    bool hasIntersectB = checkForLightIntersectWithShape(lightSource, otherEntityRectangleShape, lineIndex, 2, 3);
                    bool hasIntersectC = checkForLightIntersectWithShape(lightSource, otherEntityRectangleShape, lineIndex, 1, 2);
                    bool hasIntersectD = checkForLightIntersectWithShape(lightSource, otherEntityRectangleShape, lineIndex, 0, 1);

                    bool isRayCollidingWithSideOfShape = hasIntersectA || hasIntersectB || hasIntersectC || hasIntersectD;
                    if (isRayCollidingWithSideOfShape)
                    {
                        continue;
                    }

                    Vec2 lightSourceRayStartPos = {lightSource.rayStartVertices[lineIndex].position.x, lightSource.rayStartVertices[lineIndex].position.y};
                    Vec2 lightSourceRayEndPos = {lightSource.rayEndVertices[lineIndex].position.x, lightSource.rayEndVertices[lineIndex].position.y};
                    Crucible::LightRayIntersect windowBorderIntersectionA = isLineIntersecting(false, lightSourceRayStartPos, lightSourceRayEndPos, Vec2(0, 0), Vec2(Crucible::WINDOW_WIDTH, 0));
                    Crucible::LightRayIntersect windowBorderIntersectionB = isLineIntersecting(false, lightSourceRayStartPos, lightSourceRayEndPos, Vec2(0, Crucible::WINDOW_HEIGHT), Vec2(Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT));
                    Crucible::LightRayIntersect windowBorderIntersectionC = isLineIntersecting(false, lightSourceRayStartPos, lightSourceRayEndPos, Vec2(0, 0), Vec2(0, Crucible::WINDOW_HEIGHT));
                    Crucible::LightRayIntersect windowBorderIntersectionD = isLineIntersecting(false, lightSourceRayStartPos, lightSourceRayEndPos, Vec2(Crucible::WINDOW_WIDTH, 0), Vec2(Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT));
                    if (windowBorderIntersectionA.hasIntersection)
                    {
                        lightSource.lightRayIntersects[lineIndex].emplace_back(windowBorderIntersectionA);
                    }
                    if (windowBorderIntersectionB.hasIntersection)
                    {
                        lightSource.lightRayIntersects[lineIndex].emplace_back(windowBorderIntersectionB);
                    }
                    if (windowBorderIntersectionC.hasIntersection)
                    {
                        lightSource.lightRayIntersects[lineIndex].emplace_back(windowBorderIntersectionC);
                    }
                    else if (windowBorderIntersectionD.hasIntersection)
                    {
                        lightSource.lightRayIntersects[lineIndex].emplace_back(windowBorderIntersectionD);
                    }
                }
            }
        }
    }
}

bool CollisionSystem::isCollidingAABB(const Component::CShape& entityRect,
        const Component::CShape& otherEntityRect, sf::FloatRect& overlap)
{
    return entityRect.vertices.getBounds().intersects(otherEntityRect.vertices.getBounds(), overlap);
}

Crucible::LightRayIntersect CollisionSystem::isLineIntersecting(bool isShapeCollision, Vec2 vertexA, Vec2 vertexB, Vec2 vertexC, Vec2 vertexD)
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
        const Vec2& collisionVector = Vec2(collisionVertexX, collisionVertexY);
        return {true, collisionVector};
    }

    return {false, Vec2(0, 0)};
}

bool CollisionSystem::checkForLightIntersectWithShape(Component::CLightSource& lightSource,
        Component::CShape otherEntityRectangleShape,
        size_t lineIndex,
        size_t shapeLineStartIndex, size_t shapeLineEndIndex)
{
    Vec2 lightSourceRayStartPos = {lightSource.rayStartVertices[lineIndex].position.x, lightSource.rayStartVertices[lineIndex].position.y};
    Vec2 lightSourceRayEndPos = {lightSource.rayEndVertices[lineIndex].position.x, lightSource.rayEndVertices[lineIndex].position.y};

    sf::Vertex& otherShapeStartVert = otherEntityRectangleShape.vertices[shapeLineStartIndex];
    sf::Vertex& otherShapeEndVert = otherEntityRectangleShape.vertices[shapeLineEndIndex];
    Vec2 shapeLineStartPos{otherShapeStartVert.position.x, otherShapeStartVert.position.y};
    Vec2 shapeLineEndPos{otherShapeEndVert.position.x, otherShapeEndVert.position.y};

    Crucible::LightRayIntersect shapeLightRayIntersection = isLineIntersecting(true, lightSourceRayStartPos, lightSourceRayEndPos, shapeLineStartPos, shapeLineEndPos);

    if (shapeLightRayIntersection.hasIntersection)
    {
        lightSource.lightRayIntersects[lineIndex].emplace_back(shapeLightRayIntersection);
    }
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