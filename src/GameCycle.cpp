#include "GameCycle.h"

GameCycle::GameCycle(size_t field_height, size_t field_width): 
    field(Field(field_height, field_width)),
    renderer(ConsoleRenderer()){}

void GameCycle::run(){

    while(true){



        renderer.render(field);
    }

}