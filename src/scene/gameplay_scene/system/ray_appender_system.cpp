#include "ray_appender_system.h"

RayAppenderSystem::RayAppenderSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void RayAppenderSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (auto& entity : entities)
    {
        if (!entity.hasComponent<Component::CLightSource>())
        {
            continue;
        }

        auto& playerLightSource = entity.getComponent<Component::CLightSource>();
        auto& playerTransform = entity.getComponent<Component::CTransform>();

        int otherShapeIndex = 0;
        for (auto & otherEntity : entities)
        {
            if (entity.getId() == otherEntity.getId())
            {
                continue;
            }

            auto& entityRectangleShape = otherEntity.getComponent<Component::CShape>();
            size_t totalShapeVertices = (entityRectangleShape.vertices.getVertexCount() - 1);
            int totalRaysPerVertex = 4;
            size_t totalAdditionalRaysPerEntity = totalShapeVertices * totalRaysPerVertex;

            for (size_t vertIndex = 0; vertIndex < totalAdditionalRaysPerEntity; vertIndex+= totalRaysPerVertex)
            {
                const sf::Vertex& v = entityRectangleShape.vertices[vertIndex/totalRaysPerVertex];
                
                // Update ray vector to point left and right of endV with large scale.

                int TOTAL_CORE_RAYS = 20;
                size_t additionalRayIndex = TOTAL_CORE_RAYS + vertIndex + (totalAdditionalRaysPerEntity * otherShapeIndex);

                size_t rayDelta = 1;
                const Crucible::Vec2& rayEndPositionLeftOfVertexA = Crucible::Vec2(v.position.x-rayDelta, v.position.y);
                const Crucible::Vec2& rayEndPositionRightOfVertexA = Crucible::Vec2(v.position.x+rayDelta, v.position.y);
                const Crucible::Vec2& rayEndPositionLeftOfVertexB = Crucible::Vec2(v.position.x, v.position.y-rayDelta);
                const Crucible::Vec2& rayEndPositionRightOfVertexB = Crucible::Vec2(v.position.x, v.position.y+rayDelta);

                // Scale rays
                size_t scale = 100;
                Crucible::Vec2 playerPosition{playerTransform.position->x, playerTransform.position->y};
                Crucible::Vec2 endVa = ((rayEndPositionLeftOfVertexA - playerPosition) * scale) + playerPosition;
                Crucible::Vec2 endVb = ((rayEndPositionRightOfVertexA - playerPosition) * scale) + playerPosition;
                Crucible::Vec2 endVc = ((rayEndPositionLeftOfVertexB - playerPosition) * scale) + playerPosition;
                Crucible::Vec2 endVd = ((rayEndPositionRightOfVertexB - playerPosition) * scale) + playerPosition;

                // @Review: Look to improve on this.
                // Update rays each frame in-place
                playerLightSource.rays[additionalRayIndex] = Crucible::Ray(playerTransform.position, endVa);
                playerLightSource.rays[additionalRayIndex+1] = Crucible::Ray(playerTransform.position, endVb);
                playerLightSource.rays[additionalRayIndex+2] = Crucible::Ray(playerTransform.position, endVc);
                playerLightSource.rays[additionalRayIndex+3] = Crucible::Ray(playerTransform.position, endVd);
            }

            otherShapeIndex++;
        }
    }
}
