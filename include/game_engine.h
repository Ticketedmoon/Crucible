#pragma once

#ifndef CRUCIBLE_GAME_ENGINE_H
#define CRUCIBLE_GAME_ENGINE_H

#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>

#include <string_view>
#include <memory>
#include <iostream>

#include "common_constants.h"
#include "scene.h"
#include "gameplay_scene.h"

class GameEngine
{
    public:
        GameEngine();
        ~GameEngine();
        void startGameLoop();
        void changeScene(Scene::Type sceneType, const std::shared_ptr<Scene>& scene);

    private:
        void handleInput();
        void update(double& currentTime, double& accumulator, size_t& totalFrames);
        void render();

        void createGameWindow();
        void logDebugInfo(const size_t& totalFrames, double frameTime) const;

    public:
        sf::RenderWindow window;
        sf::Clock gameClock;
        AudioManager* m_audioManager = AudioManager::getInstance();

    private:
        Scene::Type currentScene = Scene::Type::LEVEL_ONE_GAMEPLAY_SCENE;
        std::unordered_map<Scene::Type, std::shared_ptr<Scene>> gameScenes;
};

#endif //CRUCIBLE_GAME_ENGINE_H