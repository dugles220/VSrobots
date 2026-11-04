#include "LevelSystem.h"

int LevelSystem::get_required_exp(int level){
    return level * LEVEL_COST;
}

LevelBonus LevelSystem::get_level_bonus(int){

    return {
        .health = HEALTH_BONUS,
        .energy = ENERGY_BONUS,
        .damage = DAMAGE_BONUS
    };

}