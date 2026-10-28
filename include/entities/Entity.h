#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <memory>
#include "GameConstants.h"
#include "TurnContext.h"

class Entity{

private:
    int max_health;
    int curr_health;
    char team;

public:

    Entity(int health, char team);

    virtual void take_turn(TurnContext& context) = 0;

    virtual int get_curr_health() const;
    virtual int get_max_health() const;
    virtual char get_team() const;

    virtual void set_curr_health(int health);
    virtual void set_max_health(int health);
    virtual void set_team(char team);

    virtual bool interact(std::shared_ptr<Entity>) { return false; };

    virtual void take_damage(int value);
    virtual void take_heal(int value);

    virtual ~Entity() = default;
};

#endif