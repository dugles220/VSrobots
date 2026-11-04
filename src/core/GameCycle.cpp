#include "GameCycle.h"
#include "ConsoleRenderer.h"

#include <iostream>
#include <format>
#include <string>
#include <unistd.h>
#include <limits>

GameCycle::GameCycle(size_t field_height, size_t field_width): 
    field(Field(field_height, field_width, true)),
    renderer(std::make_unique<ConsoleRenderer>()),
    ai(AIController(field, field.spawn_robot(
                           field_width / 2,
                           field_height / 2,
                           DEFAULT_HEALTH * 5,
                           DEFAULT_ENERGY * 5,
                           DEFAULT_DAMAGE * 5,
                           DEFAULT_SPEED * 3,
                           PLAYER_TEAM))),
    ac(AbilitiesCatalog()){}

void GameCycle::run(){

    std::shared_ptr<Robot> player = std::dynamic_pointer_cast<Robot>(field.get_player());
    if(player == nullptr){
        throw "Player isn't found!\n";
    }

    process_ability_pick(*player);

    update_visibility(player);
    renderer->render(field);

    while(!is_game_over(player)){

        process_start_turn();
        process_player_input(player);
        process_enemies(*player);
        process_end_turn();

        turn++;

        renderer->render(field);

        std::cout << "HEALTH: " << player->get_curr_health();
        std::cout << "/" << player->get_max_health() << std::endl;

        std::cout << "ENERGY: " << player->get_curr_energy();
        std::cout << "/" << player->get_max_energy() << std::endl;

        std::cout << "TURN: " << turn << std::endl;
    }
}

void GameCycle::process_enemies(Robot& player){

    for(const auto& entity : field.get_entities()){

        if(entity == nullptr ||
           entity->get_team() != ENEMY_TEAM) continue;

        if(entity->get_curr_health() == 0){
            field.delete_entity(entity);
            continue;
        }

        TurnContext context = {field, get_turn(), ai, player};

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

void GameCycle::process_player_input(std::shared_ptr<Robot> player){

    char input = BAD_INPUT;
    std::string good_symbs = "wasdnbWASDNB";

    int available_moves = player->get_speed();

    size_t px, py;
    field.get_entity_pos(player.get(), px, py);

    int levels_gained = 0;

    while(available_moves > 0){

        std::cout << "Move! Use WASD or N to stay.\n";

        while(input == BAD_INPUT){
            std::cin >> input;
            if(good_symbs.find(input) == std::string::npos){ input = BAD_INPUT; } 
        }

        input = tolower(input);

        size_t new_x = px;
        size_t new_y = py;

        switch (input){
            // w и s поменяны местами, т.к. начало координат слева сверху
            case 's':
                new_y += 1;
                break;
            case 'a':
                new_x -= 1;
                break;
            case 'w':
                new_y -= 1;
                break;
            case 'd':
                new_x += 1; 
                break;
            case 'n':
                available_moves = 0;
                continue;
            case 'b':
                process_ability_use(*player);
                field.get_entity_pos(player.get(), px, py);
                input = BAD_INPUT;
                continue;
        }
        
        if(!field.are_coord_valid(new_x, new_y)){
            std::cout << "End of the map! Pick another direction!\n";
            input = BAD_INPUT; 
            continue;
        }

        Cell& cell = field.get_cell(new_x, new_y);

        if(cell.get_passability() == false){
            std::cout << "There is a wall on the way!\n";
            input = BAD_INPUT;
            continue;
        }

        if(cell.is_occupied()){

            const auto& e = cell.get_entity();

            if(player->interact(e)){
                if(e->is_dead()){
                    levels_gained = player->add_exp(e->get_exp_earn());
                }

                available_moves = 0;

                if(levels_gained > 0){
                    for(int i = 0; i < levels_gained; i++){
                        process_ability_pick(*player);
                    }
                }
                break;
            }
            else{
                std::cout << "Not enough energy to attack!\n";
            }
        } else{
            int move_cost = cell.get_pass_difficulty();

            if(move_cost <= available_moves){
                if(field.move_robot(*player, cell)){
                    px = new_x;
                    py = new_y;
                    available_moves -= move_cost;
                } else{
                    std::cout << "Not enough energy to move!\n";
                }
            } else{
                std::cout << "Not enough speed to pass the cell!\n";
            }
        }

        update_visibility(player);
        renderer->render(field);

        input = BAD_INPUT;
    }
}

bool GameCycle::is_game_over(std::shared_ptr<Robot> player){

    if(player->get_curr_health() == 0){
        std::cout << "GAME OVER!" << std::endl;
        return true;
    } else if(field.get_entities().size() == 1){
        std::cout << "YOU WON!" << std::endl;
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

            robot->update_statuses();

            for(const auto& status : robot->get_statuses()){

                status->execute_on_turn_start(robot);

            }
        }
    }   
}

void GameCycle::print_abilities(std::string msg,
                                const std::vector<std::unique_ptr<Ability>>& abilities) const {

    std::cout << msg << std::endl;

    for(size_t i = 0; i < abilities.size(); i++){
        std::cout << std::format("{:>5}. {}", i + 1, abilities[i]->get_name());
        std::cout << std::endl;
    }

}

void GameCycle::process_ability_pick(Robot& player){

    bool is_picked = false;
    std::string msg = "You're able to choose one of the following abilities:";
 
    while(!is_picked){

        const auto& abs = ac.get_catalog();

        print_abilities(msg, abs);
        std::cout << "Enter the ability number:" << std::endl;

        int num = -1;

        if (!(std::cin >> num)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            system("clear");
            std::cout << "Invalid input. Enter a number." << std::endl;
            continue;
        }

        if (num < 1 || num > (int)ac.get_catalog().size()) {
            system("clear");
            std::cout << "Incorrect number. Try again." << std::endl;
            continue;
        }

        player.add_ability(ac.get_catalog()[num - 1]->clone());
        is_picked = true;
        break;
    }
}

bool GameCycle::process_ability_use(Robot& player){

    const auto& abilities = player.get_abilities();

    for(auto& status : player.get_statuses()){
        if(status->blocks_ability()) return false;
    }

    std::string msg = "Available abilities:";
    print_abilities(msg, abilities);

    bool is_picked = false;
    int number = 0;
   
    while(!is_picked){
        if (!(std::cin >> number)) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Enter a number: ";
            continue;
        }

        if(number - 1 < 0 || number > (int)abilities.size()){
            std::cout << "Incorrect number. Try again." << std::endl;
            continue;
        }

        number--;
        is_picked = true;
        break;
    }

    const auto& ability = abilities[number];

    TurnContext context = {field, get_turn(), ai, player};
    AbilityTarget target;

    if(ability->is_cell_required() || ability->is_target_required()){

        Cell& picked_cell = process_cell_pick(player);
        target = {picked_cell.get_entity().get(), &picked_cell};

    } else target = {}; 

    return ability->use(player, context, target);
}

Cell& GameCycle::process_cell_pick(Robot& player) {

    size_t cx, cy;
    field.get_entity_pos(&player, cx, cy);

    bool is_picked = false;

    std::string good_symbs = "wasdyWASDY";
    char input = BAD_INPUT;

    std::cout << "Choose cell. Use WASD and Y to confirm.\n";

    while(!is_picked){

        while(input == BAD_INPUT){
            std::cin >> input;
            if(good_symbs.find(input) == std::string::npos){ input = BAD_INPUT; } 
        }

        input = tolower(input);

        size_t new_x = cx;
        size_t new_y = cy;

        switch (input){
            case 's':
                new_y += 1;
                break;
            case 'a':
                new_x -= 1;
                break;
            case 'w':
                new_y -= 1;
                break;
            case 'd':
                new_x += 1; 
                break;
            case 'y':
                is_picked = true;
                break;
        }
        
        if(!field.are_coord_valid(new_x, new_y)){
            input = BAD_INPUT; 
            continue;
        } else{
            cx = new_x;
            cy = new_y;
            input = BAD_INPUT;
        }
    }

    return field.get_cell(cx, cy);
}