#ifndef GAME_CYCLE_H
#define GAME_CYCLE_H

#include "Field.h"
#include "ConsoleRenderer.h"

#define DEFAULT_SPEED 1
#define DEFAULT_HEALTH 100
#define DEFAULT_DAMAGE 10
#define DEFAULT_ENERGY 100

#define P_ENERGY_INCREASE 5 
#define E_ENERGY_INCREASE 2 
#define VISIBILITY_RADIUS 3 

class GameCycle{

private:
    Field field;
    ConsoleRenderer renderer;
    size_t turn = 0;

public:

    GameCycle(size_t field_height, size_t field_width);

    void run();
    bool is_game_over(std::shared_ptr<Robot> player);

    void process_player_input(std::shared_ptr<Robot> player);
    void process_robot(std::shared_ptr<Robot> robot, std::shared_ptr<Robot> player);
    void process_factory(std::shared_ptr<Factory> factory);
    void process_enemies(std::shared_ptr<Robot> player);
    void process_end_turn(std::shared_ptr<Robot> player);
    void update_visibility(std::shared_ptr<Robot> player);

    size_t get_turn() const;

};

#endif