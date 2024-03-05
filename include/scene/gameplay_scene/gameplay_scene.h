#pragma once

#ifndef CRUCIBLE_GAMEPLAY_SCENE_H
#define CRUCIBLE_GAMEPLAY_SCENE_H

#include <SFML/Graphics.hpp>

#include "game_engine.h"
#include "scene.h"

#include "scene/gameplay_scene/entity/entity_spawner.h"

#include "system.h"
#include "animation_system.h"
#include "gameplay_render_system.h"
#include "transform_system.h"
#include "lifespan_system.h"
#include "physical_collision_system.h"
#include "lighting_system.h"
#include "ray_appender_system.h"
#include "light_collision_system.h"

#include "entity_manager.h"
#include "system_manager.h"
#include "core/manager/texture_manager.h"

class GameplayScene : public Scene
{
    public:
        explicit GameplayScene(GameEngine& gameEngine);

        void update() override;
        void render() override;
        void performAction(Action& action) override;

    private:
        void registerSystems();
        void registerActions();

    private:
        EntityManager m_entityManager;
        EntitySpawner m_entitySpawner;
        TextureManager m_textureManager;
        SystemManager m_systemManager;
        LevelManager m_levelManager;

        Crucible::GameProperties gameProperties;
};


#endif //CRUCIBLE_GAMEPLAY_SCENE_H
