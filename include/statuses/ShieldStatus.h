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

    bool is_expired() const;

    void stack(std::vector<std::unique_ptr<Status>> statuses);

};

#endif