#include "HealAbility.h"
#include "Robot.h"

HealAbility::HealAbility():
             BaseAbility(DEFAULT_HEAL_COST, HEAL_ABILITY_NAME),
             heal_value(DEFAULT_HEAL) {}

HealAbility::HealAbility(int heal_value, int energy_cost) :
                        BaseAbility(energy_cost,
                                    HEAL_ABILITY_NAME),
                        heal_value(heal_value){
    if(heal_value < 0) heal_value = 0;
}

bool HealAbility::use(Robot& robot, TurnContext&, AbilityTarget&){

    if(robot.decrease_energy(get_energy_cost())){
        robot.take_heal(heal_value);
        return true;
    }

    return false;
}