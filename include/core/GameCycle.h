#ifndef GAME_CYCLE_H
#define GAME_CYCLE_H

#include "Field.h"
#include "ConsoleRenderer.h"

#include "BurnStatus.h"
#include "ShieldStatus.h"
#include "OverloadStatus.h"
#include "SlowdownStatus.h"
#include "GameConstants.h"

#define BAD_INPUT '0'

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
    void process_start_turn();
    void process_end_turn();
    void update_visibility(std::shared_ptr<Robot> player);

    size_t get_turn() const;

};

#endif