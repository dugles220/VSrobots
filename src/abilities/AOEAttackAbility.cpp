#include "AOEAttackAbility.h"
#include "Field.h"
#include "OverloadStatus.h"

AOEAttackAbility::AOEAttackAbility():
                  BaseAbility(DEFAULT_AOE_ATTACK_COST, AOE_ATTACK_ABILITY_NAME),
                  radius(DEFAULT_AOE_ATTACK_RADIUS) {}

AOEAttackAbility::AOEAttackAbility(int energy_cost, int radius):
                  BaseAbility(energy_cost, AOE_ATTACK_ABILITY_NAME),
                  radius(radius){
    if(radius < MIN_AOE_ATTACK_RADIUS) radius = MIN_AOE_ATTACK_RADIUS;
}

bool AOEAttackAbility::use(Robot& robot, TurnContext& context, AbilityTarget&){

    if(!robot.decrease_energy(get_energy_cost())) return false;

    Field& field = context.field;

    size_t u_rx, u_ry;
    field.get_entity_pos(&robot, u_rx, u_ry);
    int rx = (int)u_rx;
    int ry = (int)u_ry;

    for(int y = ry - radius; y <= ry + radius; y++){
        if(y < 0 || y >= (int)field.get_height()) continue;
        for(int x = rx - radius; x <= rx + radius; x++){
            if(x < 0 || x >= (int)field.get_width()) continue;

            int dx = x - rx;
            int dy = y - ry;

            if(dx * dx + dy * dy <= radius * radius){

                std::shared_ptr<Entity> e = field.get_cell(x, y).get_entity();

                if (e != nullptr && e.get() != &robot){
                    e->take_damage(robot.get_damage());
                    if(const auto& robot = std::dynamic_pointer_cast<Robot>(e)){
                        robot->add_status(std::make_unique<OverloadStatus>());
                    }
                }
            }
        }
    }

    return true;
}