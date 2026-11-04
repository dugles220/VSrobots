#pragma once

#include "LevelBonus.h"

#define LEVEL_COST 100

#define HEALTH_BONUS 10
#define ENERGY_BONUS 10
#define DAMAGE_BONUS 5

class LevelSystem {
public:
    static int get_required_exp(int level);
    static LevelBonus get_level_bonus(int level);
};