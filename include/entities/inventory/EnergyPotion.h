#pragma once

#include "Item.h"
#include "Robot.h"

#define ENERGY_POTION_NAME "Energy Potion"

class EnergyPotion : public Item {
private:
    int energy_value = 100;

public:
    EnergyPotion();

    void use(Robot& robot);
};