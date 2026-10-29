#include "AIController.h"

#include "Field.h"
#include "Robot.h"
#include "Factory.h"

AIController::AIController(Field& field, std::shared_ptr<Robot> player):
    field(field), player(player){
    
    if(player == nullptr) throw "Error! Player is nullptr\n";
}

void AIController::process_robot(Robot* r, TurnContext& context){

    size_t rx, ry;
    field.get_entity_position(r, rx, ry);
    size_t px, py;
    field.get_entity_position(context.player.get(), px, py);

    int available_moves = r->get_speed();

    while(available_moves > 0){

        size_t new_x = rx, new_y = ry;
        
        if(rx < px) new_x++;
        else if(rx > px) new_x--;
        else if(ry < py) new_y++;
        else if(ry > py) new_y--;

        Cell& cell = field.get_cell(new_x, new_y);

        if(cell.get_passability() == false){
            break;
        }

        if(cell.is_occupied()){
            r->interact(cell.get_entity());
            available_moves = 0;
            break;
        }
        
        int move_cost = cell.get_pass_difficulty();

        if(move_cost <= available_moves){

            if(field.move_robot(*r, cell)){
                rx = new_x;
                ry = new_y;
                available_moves -= move_cost;
            } else{
                available_moves = 0; 
                continue;
            }
        } else {
            break;
        }
    }
}

void AIController::process_factory(Factory* f, TurnContext& context){

    size_t turn = context.turn;

    if(turn == 0 || (int)turn % f->get_performance() != 0) return;

    size_t fx, fy; // factory_x, factory_y
    field.get_entity_position(f, fx, fy);

    size_t ry = fy - 1; // robox_y
    size_t rx = fx - 1; // robot_x

    bool space_available = false;

    for(; ry < fy + 3; ry++){
        for(; rx < fx + 3; rx++){

            if(!field.are_coord_valid(rx, ry)) continue;

            Cell& cell = field.get_cell(rx, ry);

            if(!cell.is_occupied() && cell.get_passability()){
                space_available = true;
                break;
            }
        }
        if(space_available) break;
    }

    if(space_available){
        field.spawn_robot(
            rx,
            ry,
            DEFAULT_HEALTH,
            DEFAULT_ENERGY, 
            DEFAULT_DAMAGE, 
            DEFAULT_SPEED,
            ENEMY_TEAM
        ); 
    }


}