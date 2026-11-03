#include "HealAbility.h"
#include "Robot.h"
#include "ShieldStatus.h"

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

        int to_heal = robot.get_max_health() - robot.get_curr_health();

        if(to_heal < heal_value){
            robot.set_curr_health(robot.get_max_health());
            robot.add_status(std::make_unique<ShieldStatus>(heal_value - to_heal));
        } else {
            robot.take_heal(heal_value);
        }
        return true;
    }

    return false;
}