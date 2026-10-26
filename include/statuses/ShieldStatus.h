#pragma once
#ifndef OVERLOAD_STATUS_H
#define OVERLOAD_STATUS_H

#include "Status.h"

#define MAX_DAMAGE_BLOCK 100
#define DEFAULT_DAMAGE_BLOCK 10

class ShieldStatus : public Status {
private:
    size_t damage_block;
    bool is_broken;

public:
    ShieldStatus();
    ShieldStatus(size_t shield_points);

    void apply(std::shared_ptr<Robot>);
    void execute_on_turn_start(std::shared_ptr<Robot>);
    void execute_on_turn_end(std::shared_ptr<Robot>);
    bool is_expired() const;

    void stack(std::vector<std::unique_ptr<Status>> statuses);

};

#endif