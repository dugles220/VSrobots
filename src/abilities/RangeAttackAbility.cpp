#include "RangeAttackAbility.h"
#include "Robot.h"
#include "Field.h"
#include "BurnStatus.h"

#include <cmath>

RangeAttackAbility::RangeAttackAbility():
                    BaseAbility(DEFAULT_RANGE_ATTACK_COST, RANGE_ATTACK_ABILITY_NAME),
                    radius(DEFAULT_RANGE_ATTACK_RADIUS) {}

RangeAttackAbility::RangeAttackAbility(int radius, int energy_cost) :
                    BaseAbility(energy_cost, RANGE_ATTACK_ABILITY_NAME),
                    radius(radius){
    if(radius < DEFAULT_RANGE_ATTACK_RADIUS) radius = DEFAULT_RANGE_ATTACK_RADIUS;
}

bool RangeAttackAbility::use(Robot& robot, TurnContext& context, AbilityTarget& target){

    if(target.entity == nullptr) return false;

    if(!robot.decrease_energy(get_energy_cost())) return false;

    if(robot.get_team() == target.entity->get_team()) return false;
    else{

        size_t rx, ry;
        context.field.get_entity_pos(&robot, rx, ry);
        size_t tx, ty;
        context.field.get_entity_pos(target.entity, tx, ty);

        int dx, dy;

        dx = abs((int)rx - (int)tx);
        dy = abs((int)ry - (int)ty);

        if(pow(dx, 2) + pow(dy, 2) <= pow(radius, 2)){
            target.entity->take_damage(robot.get_damage());
            if(const auto& robot = dynamic_cast<Robot*>(target.entity)){
                robot->add_status(std::make_unique<BurnStatus>());
            }

            return true;
        }
    }

    return false;
}

bool RangeAttackAbility::is_target_required() const { return true; }