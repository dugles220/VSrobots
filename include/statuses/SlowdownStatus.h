#pragma once
#ifndef SLOWDOWN_STATUS_H
#define SLOWDOWN_STATUS_H

#include "Status.h"

#define DEFAULT_SLOWDOWN_DUR 1
#define MAX_SLOWDOWN_DUR 5

class SlowdownStatus : public Status {

private:
    size_t turns;

public:
    SlowdownStatus();
    SlowdownStatus(size_t turns);

    bool is_expired() const;

    void increase_turns(size_t turns_count);
    void decrease_turns(size_t turns_count);

    void set_turns(size_t value);
    size_t get_turns() const;
    
    void stack(std::vector<std::unique_ptr<Status>> statuses);

};

#endif