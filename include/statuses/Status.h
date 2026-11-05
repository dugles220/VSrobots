#pragma once

class Robot;
#include <memory>
#include <vector>
#include <string>

class Status {

private:
    std::string name;

public:
   
    Status(std::string name) : name(name) {};
    virtual ~Status() = default;

    virtual void apply(std::shared_ptr<Robot>) {}
    virtual void execute_on_turn_start(std::shared_ptr<Robot>) {}
    virtual void execute_on_turn_end(std::shared_ptr<Robot>) {}
    virtual bool is_expired() const = 0;

    virtual int absorb_damage(int incoming_damage) { return incoming_damage; }
    virtual bool blocks_ability() { return false; }

    virtual void stack(std::vector<std::unique_ptr<Status>> statuses) = 0;

    std::string get_name() const { return name; }
    virtual int get_turns() const { return -1; }

};