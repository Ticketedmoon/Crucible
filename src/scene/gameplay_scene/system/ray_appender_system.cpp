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
            int totalRaysPerVertex = 2;
            size_t totalAdditionalRaysPerEntity = totalShapeVertices * totalRaysPerVertex;

            for (size_t vertIndex = 0; vertIndex < totalAdditionalRaysPerEntity; vertIndex+= totalRaysPerVertex)
            {
                const sf::Vertex& v = entityRectangleShape.vertices[vertIndex/ totalRaysPerVertex];
                
                // Update ray vector to point left and right of endV with large scale.

                size_t rayIndex = 20 + vertIndex + (totalAdditionalRaysPerEntity * otherShapeIndex);

                size_t rayDelta = 1;
                const Crucible::Vec2& rayEndPositionLeftOfVertex = Crucible::Vec2(v.position.x-rayDelta, v.position.y);
                const Crucible::Vec2& rayEndPositionRightOfVertex = Crucible::Vec2(v.position.x+rayDelta, v.position.y);

                // Scale rays
                size_t scale = 100;
                Crucible::Vec2 playerPosition{playerTransform.position->x, playerTransform.position->y};
                Crucible::Vec2 endVa = ((rayEndPositionLeftOfVertex - playerPosition) * scale) + playerPosition;
                Crucible::Vec2 endVb = ((rayEndPositionRightOfVertex - playerPosition) * scale) + playerPosition;

                // @Review: Look to improve on this.
                // Update rays each frame in-place
                playerLightSource.rays[rayIndex] = Crucible::Ray(playerTransform.position, endVa);
                playerLightSource.rays[rayIndex+1] = Crucible::Ray(playerTransform.position, endVb);
            }

            otherShapeIndex++;
        }
    }
}
