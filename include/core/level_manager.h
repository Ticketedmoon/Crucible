#pragma once

#ifndef CRUCIBLE_LEVEL_MANAGER_H
#define CRUCIBLE_LEVEL_MANAGER_H

#include <json.hpp>
#include <fstream>
#include <iostream>

#include "level.h"

class LevelManager
{
    public:
        LevelManager();

    private:
        Level activeLevel;
};


#endif //CRUCIBLE_LEVEL_MANAGER_H
