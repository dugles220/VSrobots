#pragma once

#include "Status.h"

#define MAX_STRENGTH_DUR 15
#define STRENGTH_STATUS_NAME "Increased Damage"

class StrengthStatus : public Status {
private:
    size_t turns = 5;
    float coeff = 1.2;

public:
    StrengthStatus();

    bool is_expired() const;
    int change_damage(int original_damage);

    void increase_turns(size_t turns_count);
    void decrease_turns(size_t turns_count);

    void set_turns(size_t value);
    int get_turns() const;

    void stack(std::vector<std::unique_ptr<Status>>& statuses);

};