#pragma once
#ifndef ROBOT_H
#define ROBOT_H

#include "Entity.h"
#include <memory>

class Robot : public Entity{
private:
    int max_health = 0;
    int curr_health = 0;
    int damage = 0;
    int max_energy = 0;
    int curr_energy = 0;
    double exp = 0;
    char team = '0';
public:
    // Конструкторы и деструктор
    Robot() = default;
    Robot(int health, int energy, int damage);
    ~Robot() = default;

    // Взаимодействие, 
    void interact(std::shared_ptr<Robot> other_robot);
    void level_up();
    void take_damage(int value);
    void take_heal(int value);

    // Геттеры и сеттеры
    int get_max_health() const;
    int get_curr_health() const;
    void set_max_health(int value);
    void set_curr_health(int value);

    int get_damage() const;
    void set_damage(int value);

    int get_max_energy() const;
    int get_curr_energy() const;
    void set_max_energy(int value);
    void set_curr_energy(int value);

    double get_exp() const;
    void set_exp(double value);
    void add_exp(double to_add);

    char get_team() const;
    void set_team(char team);

};

#endif