#include "HealPotion.h"

HealPotion::HealPotion() : Item(HEAL_POTION_NAME) {}

void HealPotion::use(Robot& robot){

    if(is_used()) return;

    robot.take_heal(heal_value);
    set_used(true);    
}