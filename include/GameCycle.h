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
    bool is_game_over();

    void process_player_input();
    void process_robot(std::shared_ptr<Robot> robot);
    void process_enemies();
    void process_end_turn();
    void update_visibility();

};

#endif