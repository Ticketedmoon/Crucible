#include "animation_system.h"

AnimationSystem::AnimationSystem(EntityManager& entityManager) : m_entityManager(entityManager)
{

}

void AnimationSystem::execute()
{
    std::vector<Entity> entities = m_entityManager.getEntities();
    for (auto& entity: entities)
    {
        if (!entity.hasComponent<Component::CAnimation>())
        {
            continue;
        }

        auto& entityTransform = entity.getComponent<Component::CTransform>();
        auto& entityAnimation = entity.getComponent<Component::CAnimation>();
        auto& entityCTile = entity.getComponent<Component::CTile>();

        if (entityAnimation.animations.empty())
        {
            return;
        }

        updateAnimation(entityAnimation);
        updateTileTexture(entityCTile.tile, entityCTile.texture, entityTransform, entityAnimation);
    }
}

void AnimationSystem::updateTileTexture(
        Tile& tile,
        const std::shared_ptr<sf::Texture>& tileSheetTexture,
        const Component::CTransform& transform,
        Component::CAnimation& animation)
{
    sf::VertexArray& tileVertices = *tile.vertices;
    assert(tileVertices.getVertexCount() == 4);

    size_t tileTypeValue = static_cast<int>(animation.animations
            .at(animation.currentAnimation).animationIndexes
            .at(animation.currentAnimationFrameIdx));

    // TODO the below logic only needs to be called once for static entities, we are doing additional work here unnecessarily.
    float tu = (tileTypeValue % (tileSheetTexture->getSize().x / static_cast<uint8_t>(Crucible::TILE_SIZE)));
    float tv = tileTypeValue / (tileSheetTexture->getSize().x / static_cast<uint8_t>(Crucible::TILE_SIZE));

    float tuPositionStart = tu * transform.dimensions.x;
    float tuPositionEnd = (tu + 1) * transform.dimensions.x;
    float tvPositionStart = tv * transform.dimensions.y;
    float tvPositionEnd = (tv + 1) * transform.dimensions.y;

    tileVertices[0].texCoords = {tuPositionStart, tvPositionStart};
    tileVertices[1].texCoords = {tuPositionEnd, tvPositionStart};
    tileVertices[2].texCoords = {tuPositionEnd, tvPositionEnd};
    tileVertices[3].texCoords = {tuPositionStart, tvPositionEnd};
}

void AnimationSystem::updateAnimation(Component::CAnimation& animation)
{
    double& spriteAnimationTime = animation.animations.at(animation.currentAnimation).animationTicker.currentTime;
    double animationCompletionTime = animation.animations.at(animation.currentAnimation).animationTicker.timeUntilUpdate;
    spriteAnimationTime += Crucible::DT;

    if (spriteAnimationTime >= animationCompletionTime)
    {
        animation.currentAnimationFrameIdx++;

        uint32_t totalAnimationsForAnimationGroup =  animation.animations.at(animation.currentAnimation).animationIndexes.size();
        if (animation.currentAnimationFrameIdx == totalAnimationsForAnimationGroup)
        {
            animation.currentAnimationFrameIdx = 0;
        }
        spriteAnimationTime = 0;
    }
}
