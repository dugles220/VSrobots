#include "TeleportAbility.h"
#include "Field.h"

#include <cmath>

TeleportAbility::TeleportAbility():
                 BaseAbility(DEFAULT_TELEPORT_COST, TELEPORT_ABILITY_NAME),
                 radius(DEFAULT_TELEPORT_RADIUS) {}

TeleportAbility::TeleportAbility(int radius, int energy_cost) :
                 BaseAbility(energy_cost, TELEPORT_ABILITY_NAME),
                 radius(radius){
    if(radius < MIN_TELEPORT_RADIUS) this->radius = MIN_TELEPORT_RADIUS;
}


bool TeleportAbility::use(Robot& robot, TurnContext& context, AbilityTarget& target){

    if(robot.decrease_energy(get_energy_cost()) ||
       target.cell->is_occupied() ||
       !target.cell->get_passability()) return false;
    else{
       
        size_t rx, ry;
        context.field.get_entity_pos(&robot, rx, ry);
        int cx, cy;
        bool is_cell_found = context.field.get_cell_pos(target.cell, cx, cy);
        
        if(!is_cell_found) return false; 
        int dx, dy;

        dx = abs((int)rx - cx);
        dy = abs((int)ry - cy);

        if(pow(dx, 2) + pow(dy, 2) <= pow(radius, 2)){
            context.field.move_robot(robot, *target.cell);
            return true;
        } else return false;
    }    
}

bool TeleportAbility::is_cell_required() const { return true; }

void TeleportAbility::upgrade(){

    int new_cost = get_energy_cost() - TELEPORT_UPGRADE_SIZE;

    if(new_cost < MIN_TELEPORT_COST){ 
        set_energy_cost(MIN_TELEPORT_COST);
    } else{
        set_energy_cost(new_cost);
    }

}