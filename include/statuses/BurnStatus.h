#pragma once
#ifndef BURN_STATUS_H
#define BURN_STATUS_H

#include "Status.h"

#define DEFAULT_BURN_DAMAGE 10
#define MAX_BURN_DAMAGE 20
#define DEFAULT_BURN_DUR 5
#define MAX_BURN_DUR 10

class BurnStatus : public Status {

private:
    size_t damage;
    size_t turns;

public:

    BurnStatus();
    BurnStatus(size_t damage, size_t turns);

    void apply(std::shared_ptr<Robot>);
    void execute_on_turn_start(std::shared_ptr<Robot> robot);
    void execute_on_turn_end(std::shared_ptr<Robot>);
    bool is_expired() const;

    size_t get_damage() const;
    void set_damage(size_t value);

    void increase_turns(size_t turns_count);
    void decrease_turns(size_t turns_count);

    void set_turns(size_t value);
    size_t get_turns() const;

    void stack(std::vector<std::unique_ptr<Status>> statuses);

};

#endif