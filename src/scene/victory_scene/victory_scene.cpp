#include "victory_scene/victory_scene.h"

VictoryScene::VictoryScene(GameEngine& engine) : Scene(engine)
{
    registerActions();
    registerSystems();
}

void VictoryScene::update()
{
    m_entityManager.update();
    m_systemManager.update(gameProperties);
}

void VictoryScene::render()
{
    gameEngine.renderTexture.clear(LEVEL_BACKGROUND_COLOR);

    m_systemManager.render();
    gameEngine.renderTexture.display();
    gameEngine.renderSprite.setTexture(gameEngine.renderTexture.getTexture());

    gameEngine.window.clear();
    gameEngine.window.draw(gameEngine.renderSprite);
    gameEngine.window.display();
}

void VictoryScene::performAction(Action& action)
{
    if (Action::Type::SCENE_EXIT == action.getType())
    {
        if (action.getMode() == Action::Mode::RELEASE)
        {
            return;
        }
        gameEngine.window.close();
    }
}

void VictoryScene::registerActions()
{
    // Escape
    registerActionType(sf::Keyboard::Key::Escape, Action::Type::SCENE_EXIT);
}

void VictoryScene::registerSystems()
{
    // Render
    m_systemManager.registerSystem(
            std::make_shared<VictoryRenderSystem>(gameEngine.renderTexture, m_entityManager), SystemManager::SystemType::RENDER);
}