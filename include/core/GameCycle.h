#pragma once

#include "Field.h"
#include "Renderer.h"
#include "AIController.h"

#include "AbilitiesCatalog.h"

#include "BurnStatus.h"
#include "ShieldStatus.h"
#include "OverloadStatus.h"
#include "SlowdownStatus.h"
#include "GameConstants.h"

#define BAD_INPUT '0'

class GameCycle{

private:
    Field field;
    std::unique_ptr<Renderer> renderer;
    size_t turn = 0;
    AIController ai;
    AbilitiesCatalog ac;

public:

    GameCycle(size_t field_height, size_t field_width);

    void run();
    bool is_game_over(std::shared_ptr<Robot> player);

    void process_player_input(std::shared_ptr<Robot> player);
    void process_enemies(Robot& player);
    void process_start_turn();
    void process_end_turn();
    void update_visibility(std::shared_ptr<Robot> player);

    void print_abilities(std::string msg,
                         const std::vector<std::unique_ptr<Ability>>& abilities) const;
    void process_ability_pick(Robot& player);
    bool process_ability_use(Robot& player);
    Cell& process_cell_pick(Robot& player);

    size_t get_turn() const;

};