#pragma once
#ifndef STATUS_H
#define STATUS_H

class Robot;
#include <memory>
#include <vector>

class Status {

public:
    
    virtual ~Status() = default;

    virtual void apply(std::shared_ptr<Robot>) {}
    virtual void execute_on_turn_start(std::shared_ptr<Robot>) {}
    virtual void execute_on_turn_end(std::shared_ptr<Robot>) {}
    virtual bool is_expired() const = 0;

    virtual int absorb_damage(int incoming_damage) { return incoming_damage; }

    virtual void stack(std::vector<std::unique_ptr<Status>> statuses) = 0;

};

#endif