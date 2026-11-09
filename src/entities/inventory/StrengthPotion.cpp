#include "StrengthPotion.h"
#include "StrengthStatus.h"
#include "Robot.h"

StrengthPotion::StrengthPotion() : Item(STRENGTH_POTION_NAME) {}

void StrengthPotion::use(Robot& robot){
    if(is_used()) return;

    robot.add_status(std::make_unique<StrengthStatus>());

    set_used(true);
}