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
                    checkForLightIntersectWithShape(otherEntityRectangleShape, lightSource, lineIndex);

                    checkForLightIntersectWithWindowBorderSide(lightSource, lineIndex, Vec2(0, 0), Vec2(Crucible::WINDOW_WIDTH, 0));
                    checkForLightIntersectWithWindowBorderSide(lightSource, lineIndex, Vec2(0, Crucible::WINDOW_HEIGHT), Vec2(Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT));
                    checkForLightIntersectWithWindowBorderSide(lightSource, lineIndex, Vec2(0, 0), Vec2(0, Crucible::WINDOW_HEIGHT));
                    checkForLightIntersectWithWindowBorderSide(lightSource, lineIndex, Vec2(Crucible::WINDOW_WIDTH, 0), Vec2(Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT));
                }
            }
        }
    }
}

void CollisionSystem::checkForLightIntersectWithWindowBorderSide(Component::CLightSource& lightSource, int lineIndex,
        Vec2 windowBorderVertexA, Vec2 windowBorderVertexB)
{
    Vec2 rayStartPos = {lightSource.rayStartVertices[lineIndex].position.x, lightSource.rayStartVertices[lineIndex].position.y};
    Vec2 rayEndPos = {lightSource.rayEndVertices[lineIndex].position.x, lightSource.rayEndVertices[lineIndex].position.y};
    Crucible::LightRayIntersect windowBorderIntersectionA = isLineIntersecting(false, rayStartPos, rayEndPos, windowBorderVertexA, windowBorderVertexB);
    if (windowBorderIntersectionA.hasIntersection)
    {
        std::vector<Crucible::LightRayIntersect>& intersects = lightSource.lightRayIntersects[lineIndex];
        intersects.emplace_back(windowBorderIntersectionA);
    }
}

void CollisionSystem::checkForLightIntersectWithShape(Component::CShape& otherEntityShape,
        Component::CLightSource& lightSource, int lineIndex)
{
    // @Refactor: Rather than order these in reverse, sort by closest distance to line for a more scalable solution.
    size_t vertexCount = otherEntityShape.vertices.getVertexCount();
    for (int i = 0; i < vertexCount-1; i++)
    {
        bool hasIntersect = checkForLightIntersectWithShapeSide(lightSource, otherEntityShape,lineIndex, i, i + 1);
        if (hasIntersect)
        {
            return;
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
        return {true, isShapeCollision, collisionVector, vertexC, vertexD};
    }

    return {false, isShapeCollision, Vec2(0, 0), vertexC, vertexD};
}

bool CollisionSystem::checkForLightIntersectWithShapeSide(Component::CLightSource& lightSource,
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
    return a.x * b.y - b.x * a.y;
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