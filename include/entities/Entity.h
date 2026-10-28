#pragma once
#ifndef ENTITY_H
#define ENTITY_H

#include <memory>

class Entity{

private:
    int max_health;
    int curr_health;
    char team;

public:

    Entity(int health, char team);

    int get_curr_health() const;
    int get_max_health() const;
    char get_team() const;

    void set_curr_health(int health);
    void set_max_health(int health);
    void set_team(char team);

    bool interact(std::shared_ptr<Entity> entity);

    void take_damage(int value);
    void take_heal(int value);

    virtual ~Entity() = default;
};

#endif