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

        process_player_input();
        process_enemies();
        process_end_turn();

        renderer.render(field);
    }
}

void GameCycle::process_enemies(){

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