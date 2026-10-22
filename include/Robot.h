#pragma once
#ifndef ROBOT_H
#define ROBOT_H

#include "Entity.h"

#define MOVE_COST 5
#define ATTACK_COST 10
#define MAX_SPEED 5

class Robot : public Entity{
private:

    int damage;
    int speed;

    int max_energy;
    int curr_energy;

    double exp;

public:

    Robot(int health, int energy, int damage, int speed, char team);
    ~Robot() = default;

    void interact(std::shared_ptr<Entity> entity);
    void level_up();

    void increase_energy(int value);
    bool decrease_energy(int value);

    int get_speed() const;
    void set_speed(int speed);

    int get_damage() const;
    void set_damage(int value);

    int get_max_energy() const;
    int get_curr_energy() const;
    void set_max_energy(int value);
    void set_curr_energy(int value);

    double get_exp() const;
    void set_exp(double value);
    void add_exp(double to_add);

};

#endif