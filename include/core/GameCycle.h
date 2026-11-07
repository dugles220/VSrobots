#pragma once

#include <string>

#include "Field.h"
#include "Renderer.h"
#include "AIController.h"
#include "PlayerCommand.h"
#include "InputMode.h"
#include "TurnPhase.h"
#include "EventLog.h"

#include "AbilitiesCatalog.h"

#include "BurnStatus.h"
#include "ShieldStatus.h"
#include "OverloadStatus.h"
#include "SlowdownStatus.h"
#include "GameConstants.h"

class GameCycle{

private:
    Field field;
    TurnPhase turn_phase = TurnPhase::StartTurn;
    size_t turn = 0;
    AIController ai;
    AbilitiesCatalog ac;
    InputMode input_mode = InputMode::AbilityPick;
    EventLog log;

    int available_moves = 0;

public:

    GameCycle(size_t field_height, size_t field_width);

    void update();
    bool is_game_over();

    void process_input(PlayerCommand command);
    void process_player_action(PlayerCommand command);
    void process_enemies();
    void process_start_turn();
    void process_end_turn();
    void update_visibility(std::shared_ptr<Robot> player);

    void process_ability_pick(int index);
    bool process_ability_use(int index, Cell* picked_cell);

    size_t get_turn() const;
    Field& get_field();
    InputMode get_input_mode() const;
    void set_input_mode(InputMode mode);
    const EventLog& get_log();

};