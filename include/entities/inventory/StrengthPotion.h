#pragma once

#include "Item.h"

#define STRENGTH_POTION_NAME "Strength Potion"

class StrengthPotion : public Item {
public:
    StrengthPotion();

    void use(Robot& robot);
};