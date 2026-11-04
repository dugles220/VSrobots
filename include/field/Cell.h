#pragma once

#include "Entity.h"
#include "Status.h"
#include "BurnStatus.h"
#include "ShieldStatus.h"
#include "OverloadStatus.h"
#include "SlowdownStatus.h"

#include <vector>
#include <random>

#define MIN_PASS_DIFF 1
#define MAX_PASS_DIFF 3

class Cell{

private:

    size_t pass_difficulty = MIN_PASS_DIFF;
    bool is_passable = true;
    bool is_visible = false;
    std::shared_ptr<Entity> entity = nullptr;
    std::vector<std::unique_ptr<Status>> statuses;

public:

    Cell();
    ~Cell() = default;

    Cell(const Cell&) = delete;
    Cell& operator=(const Cell&) = delete;

    Cell(Cell&&) noexcept = default;
    Cell& operator=(Cell&&) noexcept = default;


    bool get_passability() const;
    bool get_visibility() const;
    bool is_occupied() const;
    std::shared_ptr<Entity> get_entity() const;
    size_t get_pass_difficulty() const;

    void set_entity(std::shared_ptr<Entity> entity);    
    void set_passability(bool value);
    void set_visibility(bool value);
    void set_pass_difficulty(int value);

    void generate_statuses(std::mt19937& gen);
    void transfer_status(Robot& robot, std::mt19937& gen);
    std::unique_ptr<Status> generate_random_status(std::mt19937& gen);

};