#include "GameCycle.h"

#include <iostream>
#include <string>
#include <unistd.h>

GameCycle::GameCycle(size_t field_height, size_t field_width): 
    field(Field(field_height, field_width)),
    renderer(ConsoleRenderer()){
        field.spawn_robot(
            field_height / 2,
            field_width / 2,
            DEFAULT_HEALTH * 5,
            DEFAULT_ENERGY * 5,
            DEFAULT_DAMAGE * 5,
            DEFAULT_SPEED * 3,
            PLAYER_TEAM);
        field.init_demo_level();
    }

void GameCycle::run(){


    std::shared_ptr<Robot> player = std::dynamic_pointer_cast<Robot>(field.get_player());
    if(player == nullptr){
        throw "Player isn't found!\n";
    }

    update_visibility(player);
    renderer.render(field);

    while(!is_game_over(player)){

        process_start_turn();
        process_player_input(player);
        process_enemies(player);
        process_end_turn();

        turn++;

        renderer.render(field);

        std::cout << "HEALTH: " << player->get_curr_health();
        std::cout << "/" << player->get_max_health() << std::endl;

        std::cout << "ENERGY: " << player->get_curr_energy();
        std::cout << "/" << player->get_max_energy() << std::endl;

        std::cout << "TURN: " << turn << std::endl;
    }
}

void GameCycle::process_enemies(std::shared_ptr<Robot> player){

    for(const auto& entity : field.get_entities()){

        if(entity == nullptr ||
           entity->get_team() != ENEMY_TEAM) continue;

        if(entity->get_curr_health() == 0){
            field.delete_entity(entity);
            continue;
        }

        if(std::shared_ptr<Robot> robot = std::dynamic_pointer_cast<Robot>(entity)){
            process_robot(robot, player);
        }
        else if(std::shared_ptr<Factory> factory = std::dynamic_pointer_cast<Factory>(entity)){
            process_factory(factory);
        }
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

    std::string good_symbs = "wasdnWASDN";

    int available_moves = player->get_speed();

    size_t px, py;

    field.get_entity_position(player, px, py);

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
            if(player->interact(cell.get_entity())){
                available_moves = 0;
                break;
            }
            else{
                std::cout << "Not enough energy to attack!\n";
            }
        } else{
            int move_cost = cell.get_pass_difficulty();

            if(move_cost <= available_moves){
                if(field.move_robot(player, cell)){
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
        renderer.render(field);

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
    field.get_entity_position(player, u_px, u_py); 

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

void GameCycle::process_robot(std::shared_ptr<Robot> robot, std::shared_ptr<Robot> player){

    size_t rx, ry;
    field.get_entity_position(robot, rx, ry);
    size_t px, py;
    field.get_entity_position(player, px, py);

    int available_moves = robot->get_speed();

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
            robot->interact(cell.get_entity());
            available_moves = 0;
            break;
        }
        
        int move_cost = cell.get_pass_difficulty();

        if(move_cost <= available_moves){

            if(field.move_robot(robot, cell)){
                rx = new_x;
                ry = new_y;
                available_moves -= move_cost;
            } else{
                available_moves = 0; 
                continue;
            }
        } else {
            renderer.render(field);
            break;
        }
        renderer.render(field);
        usleep(600000);

    }
}

void GameCycle::process_factory(std::shared_ptr<Factory> factory){

    if(turn == 0 || (int)turn % factory->get_performance() != 0) return;

    size_t fx, fy; // factory_x, factory_y
    field.get_entity_position(factory, fx, fy);

    size_t ry = fy - 1; // robox_y
    size_t rx = fx - 1; // robot_x

    bool space_available = false;

    for(; ry < ry + 3; ry++){
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

    renderer.render(field);

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