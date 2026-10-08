#include "GameCycle.h"

#include <iostream>
#include <string>

GameCycle::GameCycle(size_t field_height, size_t field_width): 
    field(Field(field_height, field_width)),
    renderer(ConsoleRenderer()),
    x_player(field_width / 2),
    y_player(field_height / 2),
    player(field.spawn_robot(x_player, y_player, 100, 100, 20, 'P')){}

void GameCycle::run(){

    renderer.render(field);

    while(true){

        if(player->get_curr_health() == 0){
            std::cout << "GAME OVER!" << std::endl;
            break;
        } else if(field.get_entities().capacity() == 1){
            std::cout << "YOU WON!" << std::endl;
            break;
        }

        process_player_input();
        process_enemies();
        process_end_turn();

        renderer.render(field);
        std::cout << "HEALTH: " << player->get_max_health() << "/" << player->get_max_health() << std::endl;
        std::cout << "ENERGY: " << player->get_curr_energy() << "/" << player->get_max_energy() << std::endl;
    }
}

void GameCycle::process_enemies(){

    size_t px, py;

    if(!field.get_entity_position(player, px, py)) return;

    for(const auto& entity : field.get_entities()){

        if(entity == player || entity->get_team() != 'E') continue;

        size_t ex, ey;

        field.get_entity_position(entity, ex, ey);

        size_t next_ex = ex;
        size_t next_ey = ey;

        if(ex < px) next_ex++;
        else if(ex > px) next_ex--;
        else if(ey < py) next_ey++;
        else if(ey > py) next_ey--;

        field.move_robot(entity, next_ex, next_ey);
        
    }

}

void GameCycle::process_end_turn(){

}

void GameCycle::process_player_input(){

    char input = '0';

    std::string good_symbs = "wasd";

    while(input == '0'){

        std::cin >> input;

        if(good_symbs.find(input) == std::string::npos){
            input = '0';
        } 
    }

    switch (input){
        // w и s поменяны местами, т.к. начало координат слева сверху
        // а мне лень это фиксить на данном этапе
        case 's':
            if (field.move_robot(player, x_player, y_player + 1)) y_player += 1;
            break;
        case 'a':
            if (field.move_robot(player, x_player - 1, y_player)) x_player -= 1;
            break;
        case 'w':
            if (field.move_robot(player, x_player, y_player - 1)) y_player -= 1;
            break;
        case 'd':
            if (field.move_robot(player, x_player + 1, y_player)) x_player += 1;
            break;
    }

}