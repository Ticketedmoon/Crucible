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

            resolvePhysicalCollisions(entityRectangleShape, entityTransform, otherEntity, otherEntityRectangleShape);
            resolveLightCollisions(entity, otherEntityRectangleShape);
        }
    }
}

void CollisionSystem::resolveLightCollisions(const Entity& entity, Component::CShape& otherEntityRectangleShape)
{
    if (entity.hasComponent<Component::CLightSource>())
    {
        auto& lightSource = entity.getComponent<Component::CLightSource>();

        for (size_t lineIndex = 0; lineIndex <  lightSource.rays.size(); lineIndex++)
        {
            Crucible::Ray& ray = lightSource.rays[lineIndex];
            checkForLightIntersectWithShape(otherEntityRectangleShape, lightSource, ray, lineIndex);
            checkForLightIntersectWithWindowBorderSide(lightSource, ray, lineIndex, Crucible::Vec2(0, 0), Crucible::Vec2(Crucible::WINDOW_WIDTH, 0));
            checkForLightIntersectWithWindowBorderSide(lightSource, ray, lineIndex, Crucible::Vec2(0, Crucible::WINDOW_HEIGHT), Crucible::Vec2(Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT));
            checkForLightIntersectWithWindowBorderSide(lightSource, ray, lineIndex, Crucible::Vec2(0, 0), Crucible::Vec2(0, Crucible::WINDOW_HEIGHT));
            checkForLightIntersectWithWindowBorderSide(lightSource, ray, lineIndex, Crucible::Vec2(Crucible::WINDOW_WIDTH, 0), Crucible::Vec2(Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT));
        }
    }
}

void CollisionSystem::resolvePhysicalCollisions(const Component::CShape& entityRectangleShape,
        Component::CTransform& entityTransform, const Entity& otherEntity,
        const Component::CShape& otherEntityRectangleShape)
{
    sf::FloatRect overlap;
    if (isCollidingAABB(entityRectangleShape, otherEntityRectangleShape, overlap))
    {
        auto& otherEntityTransform = otherEntity.getComponent<Component::CTransform>();
        resolveCollision(entityRectangleShape, entityTransform, otherEntityRectangleShape, otherEntityTransform, overlap);
    }
}

void CollisionSystem::checkForLightIntersectWithWindowBorderSide(Component::CLightSource& lightSource,
        Crucible::Ray& ray, size_t lineIndex,
        Crucible::Vec2 windowBorderVertexA, Crucible::Vec2 windowBorderVertexB)
{
    Crucible::Vec2 rayStartPos = {ray.getStartVertex().x, ray.getStartVertex().y};
    Crucible::Vec2 rayEndPos = {ray.getEndVertex().x, ray.getEndVertex().y};
    Crucible::LightRayIntersect windowBorderIntersection = isLineIntersecting(false, rayStartPos, rayEndPos, windowBorderVertexA, windowBorderVertexB);
    if (windowBorderIntersection.hasIntersection)
    {
        lightSource.lightRayIntersects[lineIndex].emplace_back(windowBorderIntersection);
    }
}

void CollisionSystem::checkForLightIntersectWithShape(Component::CShape& otherEntityShape,
        Component::CLightSource& lightSource, Crucible::Ray& ray, size_t lineIndex)
{
    // @Refactor: Rather than order these in reverse, sort by closest distance to line for a more scalable solution.
    for (size_t i = 0; i < otherEntityShape.vertices.getVertexCount()-1; i++)
    {
        Crucible::Vec2 ratStartVertex = ray.getStartVertex();
        Crucible::Vec2 rayStartPos = {ratStartVertex.x, ratStartVertex.y};
        Crucible::Vec2 rayEndPos = {ray.getEndVertex().x, ray.getEndVertex().y};

        sf::Vertex& otherShapeStartVert = otherEntityShape.vertices[i];
        sf::Vertex& otherShapeEndVert = otherEntityShape.vertices[i+1];
        Crucible::Vec2 shapeLineStartPos{otherShapeStartVert.position.x, otherShapeStartVert.position.y};
        Crucible::Vec2 shapeLineEndPos{otherShapeEndVert.position.x, otherShapeEndVert.position.y};

        Crucible::LightRayIntersect shapeLightRayIntersection = isLineIntersecting(true, rayStartPos,
                rayEndPos, shapeLineStartPos, shapeLineEndPos);

        if (shapeLightRayIntersection.hasIntersection)
        {
            lightSource.lightRayIntersects[lineIndex].emplace_back(shapeLightRayIntersection);

            // First corner (Uses scale factor of first corner ray plus an amount to bypass)
            Crucible::Ray cornerRayA(ratStartVertex, {ray.getScaleFactor().x+1, ray.getScaleFactor().y});
            Crucible::Ray cornerRayB(ratStartVertex, {ray.getScaleFactor().x-1, ray.getScaleFactor().y});
            Crucible::Ray cornerRayC(ratStartVertex, {ray.getScaleFactor().x, ray.getScaleFactor().y+1});
            Crucible::Ray cornerRayD(ratStartVertex, {ray.getScaleFactor().x, ray.getScaleFactor().y-1});

            // Second corner (Uses scale factor of second corner ray plus an amount to bypass)
            Crucible::Ray cornerRayE(ratStartVertex, {ray.getScaleFactor().x+1, ray.getScaleFactor().y});
            Crucible::Ray cornerRayF(ratStartVertex, {ray.getScaleFactor().x+1, ray.getScaleFactor().y});
            Crucible::Ray cornerRayG(ratStartVertex, {ray.getScaleFactor().x+1, ray.getScaleFactor().y});
            Crucible::Ray cornerRayH(ratStartVertex, {ray.getScaleFactor().x+1, ray.getScaleFactor().y});

            size_t index = Crucible::TOTAL_CORE_LIGHT_RAYS + (i * 8);
            lightSource.rays[index] = cornerRayA;
            lightSource.rays[index + 1] = cornerRayB;
            lightSource.rays[index + 2] = cornerRayC;
            lightSource.rays[index + 3] = cornerRayD;
            lightSource.rays[index + 4] = cornerRayE;
            lightSource.rays[index + 5] = cornerRayF;
            lightSource.rays[index + 6] = cornerRayG;
            lightSource.rays[index + 7] = cornerRayH;
        }
    }
}

bool CollisionSystem::isCollidingAABB(const Component::CShape& entityRect,
        const Component::CShape& otherEntityRect, sf::FloatRect& overlap)
{
    return entityRect.vertices.getBounds().intersects(otherEntityRect.vertices.getBounds(), overlap);
}

Crucible::LightRayIntersect CollisionSystem::isLineIntersecting(bool isShapeCollision, Crucible::Vec2 vertexA, Crucible::Vec2 vertexB, Crucible::Vec2 vertexC, Crucible::Vec2 vertexD)
{
    Crucible::Vec2 r = (vertexB - vertexA);
    Crucible::Vec2 s = (vertexD - vertexC);
    Crucible::Vec2 cma = vertexC - vertexA;

    float rCrossS = crossProduct(r, s);
    float cmaCrossS = crossProduct(cma, s);
    float cmaCrossR = crossProduct(cma, r);

    float t = (cmaCrossS / rCrossS);
    float u = (cmaCrossR / rCrossS);

    if (t >= 0 && t <= 1 && u >= 0 && u <= 1)
    {
        float collisionVertexX = vertexA.x + (t * r.x);
        float collisionVertexY = vertexA.y + (t * r.y);

        const Crucible::Vec2& collisionVector = Crucible::Vec2(collisionVertexX, collisionVertexY);
        return {true, isShapeCollision, collisionVector, vertexC, vertexD};
    }

    return {false, isShapeCollision, Crucible::Vec2(0, 0), vertexC, vertexD};
}

float CollisionSystem::crossProduct(Crucible::Vec2 a, Crucible::Vec2 b)
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

    const Crucible::Vec2& entityPosition = Crucible::Vec2(entityTransform.position.x - xDiff, entityTransform.position.y + yDiff);
    const Crucible::Vec2& otherEntityPosition = Crucible::Vec2(otherEntityTransform.position.x - oxDiff, otherEntityTransform.position.y + oyDiff);
    const Crucible::Vec2& result = entityPosition - otherEntityPosition;

    applyCollisionManifoldToTransform(entityTransform, overlap, result);
}

void CollisionSystem::applyCollisionManifoldToTransform(Component::CTransform& entityTransform,
        const sf::FloatRect& overlap, const Crucible::Vec2& result)
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