#include "gameplay_scene/gameplay_scene.h"

GameplayScene::GameplayScene(GameEngine& engine) : Scene(engine),
    m_entitySpawner(m_entityManager, m_textureManager, engine.gameClock), m_levelManager(m_textureManager)
{
    registerActions();
    registerSystems();

    m_entitySpawner.createPlayer();
    m_entitySpawner.createGuard(LevelManager::GUARD_LIGHTING_LAYER_A, LevelManager::GUARD_PATHING_LAYER_A);
    //m_entitySpawner.createGuard(LevelManager::COLLISION_LAYER_PLAYER_B, LevelManager::GUARD_PATHING_LAYER_B);
}

void GameplayScene::update()
{
    m_entityManager.update();
    m_systemManager.update(gameProperties);
}

void GameplayScene::render()
{
    gameEngine.renderTexture.clear();
    m_systemManager.render();
    gameEngine.renderTexture.display();
    gameEngine.renderSprite.setTexture(gameEngine.renderTexture.getTexture());

    gameEngine.window.clear();
    gameEngine.window.draw(gameEngine.renderSprite, sf::RenderStates(sf::BlendAdd));
    gameEngine.window.display();
}

void GameplayScene::performAction(Action& action)
{
    if (Action::Type::SCENE_EXIT == action.getType())
    {
        if (action.getMode() == Action::Mode::RELEASE)
        {
            return;
        }
        gameEngine.window.close();
    }

    std::vector<Entity> controllableEntities = m_entityManager.getEntitiesByComponentType<Component::CControllable>();
    for (const Entity& e : controllableEntities)
    {
        auto& cControllable = e.getComponent<Component::CControllable>();
        if (Action::Type::MOVE_LEFT == action.getType())
        {
            cControllable.isMovingLeft = action.getMode() == Action::Mode::PRESS;
        }

        if (Action::Type::MOVE_RIGHT == action.getType())
        {
            cControllable.isMovingRight = action.getMode() == Action::Mode::PRESS;
        }

        if (Action::Type::MOVE_UP == action.getType())
        {
            cControllable.isMovingUp = action.getMode() == Action::Mode::PRESS;
        }

        if (Action::Type::MOVE_DOWN == action.getType())
        {
            cControllable.isMovingDown = action.getMode() == Action::Mode::PRESS;
        }
    }
}

void GameplayScene::registerActions()
{
    // Escape
    registerActionType(sf::Keyboard::Key::Escape, Action::Type::SCENE_EXIT);

    // Movement
    registerActionType(sf::Keyboard::Key::Left, Action::Type::MOVE_LEFT);
    registerActionType(sf::Keyboard::Key::Right, Action::Type::MOVE_RIGHT);
    registerActionType(sf::Keyboard::Key::Up, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::Key::Down, Action::Type::MOVE_DOWN);

    registerActionType(sf::Keyboard::Key::A, Action::Type::MOVE_LEFT);
    registerActionType(sf::Keyboard::Key::D, Action::Type::MOVE_RIGHT);
    registerActionType(sf::Keyboard::Key::W, Action::Type::MOVE_UP);
    registerActionType(sf::Keyboard::Key::S, Action::Type::MOVE_DOWN);
}

void GameplayScene::registerSystems()
{
    // Standard
    m_systemManager.registerSystem(
            std::make_shared<TransformSystem>(m_entityManager, gameEngine.gameClock),
            SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<PhysicalCollisionSystem>(m_entityManager, gameProperties),
            SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<AnimationSystem>(m_entityManager), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<LifespanSystem>(m_entityManager), SystemManager::SystemType::UPDATE);

    // Lighting
    m_systemManager.registerSystem(
            std::make_shared<RayAppenderSystem>(m_entityManager), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<LightCollisionSystem>(m_entityManager), SystemManager::SystemType::UPDATE);
    m_systemManager.registerSystem(
            std::make_shared<LightingSystem>(m_entityManager, m_entitySpawner, gameEngine.gameClock),
            SystemManager::SystemType::UPDATE);

    // Render
    m_systemManager.registerSystem(
            std::make_shared<GameplayRenderSystem>(gameEngine, m_entityManager, m_textureManager, gameProperties),
            SystemManager::SystemType::RENDER);
}