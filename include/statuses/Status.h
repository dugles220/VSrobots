#pragma once
#ifndef STATUS_H
#define STATUS_H

#include "Robot.h"
#include <vector>

class Status {

public:
    
    virtual ~Status() = default;

    virtual void apply(std::shared_ptr<Robot> robot) = 0;
    virtual void execute_on_turn_start(std::shared_ptr<Robot> robot) = 0;
    virtual void execute_on_turn_end(std::shared_ptr<Robot> robot) = 0;
    virtual bool is_expired() const = 0;

    virtual void stack(std::vector<std::unique_ptr<Status>> statuses) = 0;

};

#endif