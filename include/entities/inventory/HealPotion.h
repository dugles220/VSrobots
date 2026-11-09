#pragma once

#include "Item.h"
#include "Robot.h"

#define HEAL_POTION_NAME "Heal Potion"

class HealPotion : public Item{
private:
    int heal_value = 50;

public:
    HealPotion();

    void use(Robot& robot);
};