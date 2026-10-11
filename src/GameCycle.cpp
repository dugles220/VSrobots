#include "GameCycle.h"

#include <iostream>
#include <string>
#include <unistd.h>

#define P_ENERGY_INCREASE 5 
#define E_ENERGY_INCREASE 2 
#define VISIBILITY_RADIUS 3 

GameCycle::GameCycle(size_t field_height, size_t field_width): 
    field(Field(field_height, field_width)),
    renderer(ConsoleRenderer()),
    x_player(field_width / 2),
    y_player(field_height / 2),
    player(field.spawn_robot(x_player, y_player, 500, 200, 100, 3, 'P')){

        field.init_demo_level();
    }

void GameCycle::run(){

    update_visibility();
    renderer.render(field);

    while(!is_game_over()){

        process_player_input();
        process_enemies();
        process_end_turn();

        turn++;

        renderer.render(field);
        std::cout << "HEALTH: " << player->get_curr_health() << "/" << player->get_max_health() << std::endl;
        std::cout << "ENERGY: " << player->get_curr_energy() << "/" << player->get_max_energy() << std::endl;
        std::cout << "TURN: " << turn << std::endl;
    }
}

void GameCycle::process_enemies(){

    for(const auto& entity : field.get_entities()){


        if(entity == nullptr || entity == player || entity->get_team() != 'E') continue;

        if(entity->get_curr_health() == 0){
            field.delete_entity(entity);
            continue;
        }

        if(std::shared_ptr<Robot> robot = std::dynamic_pointer_cast<Robot>(entity)){
            process_robot(robot);
        }
        else if(std::shared_ptr<Factory> factory = std::dynamic_pointer_cast<Factory>(entity)){
            process_factory(factory);
        }
    }
}

void GameCycle::process_end_turn(){

    player->increase_energy(P_ENERGY_INCREASE);

    for(const auto& entity :field.get_entities()){

        if(entity == player || entity->get_team() != 'E') continue;

        if(std::shared_ptr<Robot> robot = std::dynamic_pointer_cast<Robot>(entity)){
            robot->increase_energy(E_ENERGY_INCREASE);
        }
    }
}

void GameCycle::process_player_input(){

    char input = '0';

    std::string good_symbs = "wasdnWASDN";

    int available_moves = player->get_speed();

    while(available_moves > 0){

        std::cout << "Move! Use WASD or N to stay.\n";

        while(input == '0'){
            std::cin >> input;
            if(good_symbs.find(input) == std::string::npos){ input = '0'; } 
        }

        size_t new_x = x_player;
        size_t new_y = y_player;

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
            input = '0';
            continue;
        }

        Cell& cell = field.get_cell(new_x, new_y);

        if(cell.is_occupied()){
            field.move_robot(player, new_x, new_y);
            available_moves = 0;
            break;
        }

        int move_cost = field.get_cell(new_x, new_y).get_pass_difficulty();

        if(move_cost <= available_moves){
            if(field.move_robot(player, new_x, new_y)){
                x_player = new_x;
                y_player = new_y;
                available_moves -= move_cost;
            } else{
                std::cout << "Not enough energy!\n";
            }
        } else{
            std::cout << "Not enough speed!\n";
        }

        update_visibility();
        renderer.render(field);

        input = '0';
    }
}

bool GameCycle::is_game_over(){

    if(player->get_curr_health() == 0){
        std::cout << "GAME OVER!" << std::endl;
        return true;
    } else if(field.get_entities().size() == 1){
        std::cout << "YOU WON!" << std::endl;
        return true;
    }

    return false;
}

void GameCycle::update_visibility(){

    int px = (int)x_player;
    int py = (int)y_player;

   
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

void GameCycle::process_robot(std::shared_ptr<Robot> robot){

    size_t x_robot, y_robot;
    field.get_entity_position(robot, x_robot, y_robot);

    int available_moves = robot->get_speed();


    while(available_moves > 0){

        size_t new_x = x_robot, new_y = y_robot;
        
        if(x_robot < x_player) new_x++;
        else if(x_robot > x_player) new_x--;
        else if(y_robot < y_player) new_y++;
        else if(y_robot > y_player) new_y--;

        Cell& new_cell = field.get_cell(new_x, new_y);

        if(new_cell.is_occupied()){
            field.move_robot(robot, new_x, new_y);
            available_moves = 0;
            break;
        }
        
        int move_cost = field.get_cell(new_x, new_y).get_pass_difficulty();

        if(move_cost <= available_moves){

            if(field.move_robot(robot, new_x, new_y)){
                x_robot = new_x;
                y_robot = new_y;
                available_moves -= move_cost;
            } else{
                available_moves -= 1; 
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

    size_t x_factory, y_factory;
    field.get_entity_position(factory, x_factory, y_factory);

    size_t y_robot = y_factory - 1;
    size_t x_robot = x_factory - 1;

    bool space_available = false;

    for(; y_robot < y_robot + 3; y_robot++){
        for(; x_robot < x_factory + 3; x_robot++){

            if(!field.are_coord_valid(x_robot, y_robot)) continue;

            Cell& cell = field.get_cell(x_robot, y_robot);

            if(!cell.is_occupied() && cell.get_passability()){
                space_available = true;
                break;
            }
        }
        if(space_available) break;
    }

    if(space_available){
        field.spawn_robot(x_robot, y_robot, 100, 50, 10, 2, 'E'); 
    }

    renderer.render(field);

}

size_t GameCycle::get_turn() const{ return turn; }