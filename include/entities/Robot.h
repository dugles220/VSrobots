#pragma once

#include "Entity.h"
#include "Status.h"
#include "Ability.h"
#include "GameConstants.h"
#include "TurnContext.h"

#include <memory>

#define MAX_SPEED 5

#define ROBOT_DAMAGE_EXP_COEFF 0.5
#define ROBOT_HEALTH_EXP_COEFF 0.1
#define ROBOT_ENERGY_EXP_COEFF 0.02

class Robot : public Entity{
private:

    int damage;
    int speed;

    int max_energy;
    int curr_energy;

    std::vector<std::unique_ptr<Status>> statuses;
    std::vector<std::unique_ptr<Ability>> abilities;

    int level = 1;
    int experience = 0;

public:

    Robot(int health, int energy, int damage, int speed, char team);
    ~Robot() = default;

    void take_turn(TurnContext& context);
    bool interact(std::shared_ptr<Entity> entity);
    bool attack(Entity& target);
    bool heal(Entity& target);

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

    int get_exp() const;
    int get_level() const;
    int add_exp(int to_add);
    void level_up();
    int get_exp_earn() const;

    void take_damage(int value);

    const std::vector<std::unique_ptr<Status>>& get_statuses();
    void add_status(std::unique_ptr<Status> status);
    void update_statuses();

    const std::vector<std::unique_ptr<Ability>>& get_abilities();
    void add_ability(std::unique_ptr<Ability> ability);
    

};