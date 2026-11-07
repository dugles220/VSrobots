#pragma once

#include "Status.h"

#define DEFAULT_SLOWDOWN_DUR 1
#define MAX_SLOWDOWN_DUR 5
#define SLOWDOWN_STATUS_NAME "Slowdown"

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
    int get_turns() const;
    
    void stack(std::vector<std::unique_ptr<Status>>& statuses);

};