#ifndef GAME_CYCLE_H
#define GAME_CYCLE_H

#include "Field.h"
#include "ConsoleRenderer.h"

class GameCycle{

private:
    Field field;
    ConsoleRenderer renderer;
    size_t x_player;
    size_t y_player;
    std::shared_ptr<Robot> player;

public:

    GameCycle(size_t field_height, size_t field_width);

    void run();

    void process_player_input();
    void process_enemies();
    void process_end_turn();

};

#endif