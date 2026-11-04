#pragma once

#include "Status.h"

#define MAX_DAMAGE_BLOCK 100
#define DEFAULT_DAMAGE_BLOCK 10

class ShieldStatus : public Status {
private:
    int damage_block;
    bool is_broken;

public:

    ShieldStatus();
    ShieldStatus(int shield_points);

    bool is_expired() const;

    int absorb_damage(int incoming_damage);

    void stack(std::vector<std::unique_ptr<Status>> statuses);

};