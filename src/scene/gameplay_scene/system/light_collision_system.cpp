#include "light_collision_system.h"

LightCollisionSystem::LightCollisionSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void LightCollisionSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();

    for (const Entity entity : entities)
    {
        for (const Entity otherEntity : entities)
        {
            if (entity.getId() == otherEntity.getId())
            {
                // @Refactor: Can we do this a better way?
                continue;
            }

            auto& otherEntityRectangleShape = otherEntity.getComponent<Component::CShape>();
            resolveLightCollisions(entity, otherEntityRectangleShape);
        }
    }
}

void LightCollisionSystem::resolveLightCollisions(const Entity& entity, Component::CShape& otherEntityRectangleShape)
{
    if (!entity.hasComponent<Component::CLightSource>())
    {
        return;
    }

    auto& lightSource = entity.getComponent<Component::CLightSource>();
    for (size_t rayIndex = 0; rayIndex < lightSource.rays.size(); rayIndex++)
    {
        // @Refactor: If ray collision with shape, we don't need to check window border collisions.
        Crucible::Ray& ray = lightSource.rays[rayIndex];
        checkForLightIntersectWithShape(otherEntityRectangleShape, lightSource, ray, rayIndex);

        // top
        checkForLightIntersectWithWindowBorderSide(lightSource, ray, rayIndex,
                Crucible::Vec2(0, 0),
                Crucible::Vec2(Crucible::WINDOW_WIDTH, 0));
        // right
        checkForLightIntersectWithWindowBorderSide(lightSource, ray, rayIndex,
                Crucible::Vec2(Crucible::WINDOW_WIDTH, 0),
                Crucible::Vec2(Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT));
        // bottom
        checkForLightIntersectWithWindowBorderSide(lightSource, ray, rayIndex,
                Crucible::Vec2(0, Crucible::WINDOW_HEIGHT),
                Crucible::Vec2(Crucible::WINDOW_WIDTH, Crucible::WINDOW_HEIGHT));
        // left
        checkForLightIntersectWithWindowBorderSide(lightSource, ray, rayIndex,
                Crucible::Vec2(0, 0),
                Crucible::Vec2(0, Crucible::WINDOW_HEIGHT));
    }
}

void LightCollisionSystem::checkForLightIntersectWithWindowBorderSide(Component::CLightSource& lightSource, Crucible::Ray& ray,
        size_t rayIndex, Crucible::Vec2 windowBorderVertexA, Crucible::Vec2 windowBorderVertexB)
{
    Crucible::Vec2 rayStartPos = {ray.getStartVertex()->x, ray.getStartVertex()->y};
    Crucible::Vec2 rayEndPos = {ray.getEndVertex().x, ray.getEndVertex().y};
    Crucible::LightRayIntersect windowBorderIntersection = isLineIntersecting(rayStartPos,
            rayEndPos, windowBorderVertexA, windowBorderVertexB);
    if (windowBorderIntersection.hasIntersection)
    {
        lightSource.lightRayIntersects[rayIndex].emplace_back(windowBorderIntersection);
    }
}

void LightCollisionSystem::checkForLightIntersectWithShape(Component::CShape& otherEntityShape,
        Component::CLightSource& lightSource, Crucible::Ray& ray, size_t lineIndex)
{
    // @Refactor: Rather than order these in reverse, sort by closest distance to line for a more scalable solution.
    // [0, 1] = top, [1, 2] = right, [2, 3] = bottom, [3, 4] = left
    for (size_t shapeSideIndex = 0; shapeSideIndex < otherEntityShape.vertices.getVertexCount()-1; shapeSideIndex++)
    {
        Crucible::Vec2 rayStartPos = {ray.getStartVertex()->x, ray.getStartVertex()->y};
        Crucible::Vec2 rayEndPos = {ray.getEndVertex().x, ray.getEndVertex().y};

        sf::Vertex& otherShapeStartVert = otherEntityShape.vertices[shapeSideIndex];
        sf::Vertex& otherShapeEndVert = otherEntityShape.vertices[shapeSideIndex + 1];

        Crucible::Vec2 shapeLineStartPos{otherShapeStartVert.position.x, otherShapeStartVert.position.y};
        Crucible::Vec2 shapeLineEndPos{otherShapeEndVert.position.x, otherShapeEndVert.position.y};

        Crucible::LightRayIntersect shapeLightRayIntersection = isLineIntersecting(rayStartPos, rayEndPos,
                shapeLineStartPos, shapeLineEndPos);

        if (!shapeLightRayIntersection.hasIntersection)
        {
            continue;
        }

        lightSource.lightRayIntersects[lineIndex].emplace_back(shapeLightRayIntersection);
    }
}

Crucible::LightRayIntersect LightCollisionSystem::isLineIntersecting(Crucible::Vec2 vertexA,
        Crucible::Vec2 vertexB, Crucible::Vec2 vertexC, Crucible::Vec2 vertexD)
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
        return {true, Crucible::Vec2(collisionVertexX, collisionVertexY)};
    }

    return {false, Crucible::Vec2(0, 0)};
}

float LightCollisionSystem::crossProduct(Crucible::Vec2 a, Crucible::Vec2 b)
{
    return a.x * b.y - b.x * a.y;
}