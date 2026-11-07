#include "GameCycle.h"

#include <iostream>
#include <format>
#include <string>
#include <unistd.h>
#include <limits>

GameCycle::GameCycle(size_t field_height, size_t field_width): 
    field(Field(field_height, field_width, true)),
    ai(AIController(field, field.spawn_robot(
                           field_width / 2,
                           field_height / 2,
                           DEFAULT_HEALTH * 5,
                           DEFAULT_ENERGY * 5,
                           DEFAULT_DAMAGE * 5,
                           DEFAULT_SPEED * 3,
                           PLAYER_TEAM))),
    ac(AbilitiesCatalog()){
        field.init_debug_level();
    }

void GameCycle::update(){

    if(input_mode != InputMode::Normal) {
        return;
    }

    switch(turn_phase) {
        case TurnPhase::StartTurn:
            process_start_turn();
            turn_phase = TurnPhase::PlayerTurn;
            break;
        
        case TurnPhase::PlayerTurn:
            break;

        case TurnPhase::EnemyTurn:
            process_enemies();
            turn_phase = TurnPhase::EndTurn;
            break;

        case TurnPhase::EndTurn:
            process_end_turn();
            turn++;
            turn_phase = TurnPhase::StartTurn;
            break;

        case TurnPhase::GameOver:
            break;
    }

}

void GameCycle::process_enemies(){

    const auto& player = std::dynamic_pointer_cast<Robot>(field.get_player());

    for(const auto& entity : field.get_entities()){

        if(entity == nullptr ||
           entity->get_team() != ENEMY_TEAM) continue;

        if(entity->get_curr_health() == 0){
            field.delete_entity(entity);
            continue;
        }

        TurnContext context = {field, get_turn(), ai, *player};

        entity->take_turn(context);

    }
}

void GameCycle::process_end_turn(){

    for(const auto& entity :field.get_entities()){
        if(std::shared_ptr<Robot> robot = std::dynamic_pointer_cast<Robot>(entity)){

            for(const auto& status : robot->get_statuses()){
                status->execute_on_turn_end(robot);
            }

            if(robot->get_team() == PLAYER_TEAM){ 
                robot->increase_energy(P_ENERGY_INCREASE);
            } else {
                robot->increase_energy(E_ENERGY_INCREASE);
            }
        }
    }
}

void GameCycle::process_player_action(PlayerCommand command){

    const auto& player = std::dynamic_pointer_cast<Robot>(field.get_player());

    size_t px, py;
    field.get_entity_pos(player.get(), px, py);

    int levels_gained = 0;

    size_t new_x = px;
    size_t new_y = py;

    switch (command){
        // w и s поменяны местами, т.к. начало координат слева сверху
        case PlayerCommand::MoveDown:
            new_y += 1;
            break;
        case PlayerCommand::MoveLeft:
            new_x -= 1;
            break;
        case PlayerCommand::MoveUp:
            new_y -= 1;
            break;
        case PlayerCommand::MoveRight:
            new_x += 1; 
            break;
        case PlayerCommand::Wait:
            available_moves = 0;
            turn_phase = TurnPhase::EnemyTurn;
            log.add("PLAYER remained in place", get_turn());
            return;
        case PlayerCommand::UseAbility:
            input_mode = InputMode::AbilityUse;
            return;
    }
    
    if(!field.are_coord_valid(new_x, new_y)){
        log.add("End of the map!", get_turn());
        return;
    }

    Cell& cell = field.get_cell(new_x, new_y);

    if(cell.get_passability() == false){
        log.add("There is a wall on the way!", get_turn());
        return;
    }
    else if(cell.is_occupied()){

        const auto& e = cell.get_entity();

        if(player->interact(e)){


            log.add("PLAYER interacted with entity", get_turn());

            if(e->is_dead()){
                int xp = e->get_exp_earn();
                auto message = std::format("PLAYER destroyed entity (+{} XP)", xp);
                log.add(message, get_turn());
                levels_gained = player->add_exp(xp);
            }

            available_moves = 0;

            if(levels_gained > 0){
                input_mode = InputMode::AbilityPick;
            }
        }
        else {
            log.add("Not enough energy to attack!", get_turn()); 
            return;
        }
    } 
    else {

        int move_cost = cell.get_pass_difficulty();

        if(move_cost <= available_moves){
            if(field.move_robot(*player, cell)){
                px = new_x;
                py = new_y;
                available_moves -= move_cost;
                log.add(std::format("PLAYER moved to <{}, {}>", px, py), get_turn());
            }
            else {
                log.add("Not enough energy to move!", get_turn()); 
                return;
            }
        } 
        else {
            log.add("Not enough speed to pass the cell!", get_turn());
        }
    }

    update_visibility(player);

    if(available_moves <= 0 && input_mode == InputMode::Normal){
        turn_phase = TurnPhase::EnemyTurn;
    }
}

bool GameCycle::is_game_over(){

    const auto& player = field.get_player();

    if(player->get_curr_health() == 0){
        return true;
    } else if(field.get_entities().size() == 1){
        return true;
    }

    return false;
}

void GameCycle::update_visibility(std::shared_ptr<Robot> player){

    size_t u_px, u_py; // unsigned координаты
    field.get_entity_pos(player.get(), u_px, u_py); 

    int px = (int)u_px;
    int py = (int)u_py;

   
    for(int y_shift = -VISIBILITY_RADIUS; y_shift <= VISIBILITY_RADIUS; y_shift++){

        int y = py + y_shift;

        if(y < 0 || y >= (int)field.get_height()) continue;

        for(int x_shift = -VISIBILITY_RADIUS; x_shift <= VISIBILITY_RADIUS; x_shift++){

            int x = px + x_shift;

            if(x < 0 || x >= (int)field.get_width()) continue;

            if(x_shift * x_shift + y_shift * y_shift <= VISIBILITY_RADIUS * VISIBILITY_RADIUS){

                field.get_cell(x, y).set_visibility(true);
            }
        }
    }

}

size_t GameCycle::get_turn() const{ return turn; }

void GameCycle::process_start_turn(){

    for(const auto& entity : field.get_entities()){

        if(std::shared_ptr<Robot> robot = std::dynamic_pointer_cast<Robot>(entity)){

            if(robot->get_team() == PLAYER_TEAM){
                available_moves = robot->get_speed();
            }

            robot->update_statuses();

            for(const auto& status : robot->get_statuses()){

                status->execute_on_turn_start(robot);

            }
        }
    }   
}


void GameCycle::process_ability_pick(int index){

    if(input_mode != InputMode::AbilityPick) return;

    if(index < 0 || index >= (int)ac.get_catalog().size()) return;

    auto player = std::dynamic_pointer_cast<Robot>(field.get_player());

    if(!player) return;

    player->add_ability(ac.get_catalog()[index]->clone());

    input_mode = InputMode::Normal;
}

bool GameCycle::process_ability_use(int index, Cell* picked_cell){

    const auto player = std::dynamic_pointer_cast<Robot>(field.get_player());
    if(!player) return false;

    const auto& abilities = player->get_abilities();

    if(index < 0 || index >= (int)abilities.size()) return false;

    for(auto& status : player->get_statuses()){
        if(status->blocks_ability()) {
            input_mode = InputMode::Normal;
            return false;
        }
    }

    const auto& ability = abilities[index];

    TurnContext context = {field, get_turn(), ai, *player};
    AbilityTarget target;

    if(ability->is_cell_required() || ability->is_target_required()){
        if(!picked_cell) return false;

        Entity* entity = nullptr;

        if(picked_cell->is_occupied()) entity = picked_cell->get_entity().get();

        target = {entity, picked_cell};

    } else target = {}; 

    if(!ability->use(*player, context, target)) return false;
    else{
        input_mode = InputMode::Normal;
        return true;
    }
}

// Cell& GameCycle::process_cell_pick(Robot& player) {

//     size_t cx, cy;
//     field.get_entity_pos(&player, cx, cy);

//     bool is_picked = false;

//     std::string good_symbs = "wasdyWASDY";
//     char input = BAD_INPUT;

//     std::cout << "Choose cell. Use WASD and Y to confirm.\n";

//     while(!is_picked){

//         while(input == BAD_INPUT){
//             std::cin >> input;
//             if(good_symbs.find(input) == std::string::npos){ input = BAD_INPUT; } 
//         }

//         input = tolower(input);

//         size_t new_x = cx;
//         size_t new_y = cy;

//         switch (input){
//             case 's':
//                 new_y += 1;
//                 break;
//             case 'a':
//                 new_x -= 1;
//                 break;
//             case 'w':
//                 new_y -= 1;
//                 break;
//             case 'd':
//                 new_x += 1; 
//                 break;
//             case 'y':
//                 is_picked = true;
//                 break;
//         }
        
//         if(!field.are_coord_valid(new_x, new_y)){
//             input = BAD_INPUT; 
//             continue;
//         } else{
//             cx = new_x;
//             cy = new_y;
//             input = BAD_INPUT;
//         }
//     }

//     return field.get_cell(cx, cy);
// }

Field& GameCycle::get_field() { return field; }
InputMode GameCycle::get_input_mode() const { return input_mode; }

void GameCycle::process_input(PlayerCommand command){

    switch(input_mode){
        case InputMode::Normal:
            process_player_action(command);
            break;
        
        case InputMode::AbilityPick:
            break;
        
        default:
            break;
    }
}

void GameCycle::set_input_mode(InputMode mode){ input_mode = mode; }