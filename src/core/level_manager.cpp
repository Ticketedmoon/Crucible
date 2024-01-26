#include "level_manager.h"

LevelManager::LevelManager()
{
    std::ifstream f("resources/maps/level_one_map.json");
    nlohmann::json data = nlohmann::json::parse(f);
    this->activeLevel.width = data["width"];
    this->activeLevel.height = data["height"];
    this->activeLevel.layers.name = data["layers"][0]["name"];
    this->activeLevel.layers.data = data["layers"][0]["data"].get<std::vector<uint8_t>>();
    std::cout << "Created level with width: [" << std::to_string(activeLevel.width) << "], height: [" << std::to_string(activeLevel.height) << "]\n";
}