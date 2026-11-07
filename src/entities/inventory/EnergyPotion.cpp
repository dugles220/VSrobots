#include "EnergyPotion.h"

EnergyPotion::EnergyPotion() : Item(ENERGY_POTION_NAME) {}

void EnergyPotion::use(Robot& robot){

    if(is_used()) return;

    robot.increase_energy(energy_value);
    set_used(true);
}