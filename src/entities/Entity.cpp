#include "Entity.h"

#include <stdexcept>

Entity::Entity(int health, char team):
    max_health(health), curr_health(health), team(team){

    if(health <= 0){
        throw std::invalid_argument("Health value must be greater than 0!\n");
    }
}

int Entity::get_curr_health() const { return curr_health; }
int Entity::get_max_health() const { return max_health; }
char Entity::get_team() const { return team; }

void Entity::set_curr_health(int value){
    if(value <= 0){ curr_health = 0; }
    else if (value > max_health){ curr_health = max_health; }
    curr_health = value;
}

void Entity::set_max_health(int value){
    if(value <= 0){ max_health = 0; }
    max_health = value;
    if(curr_health > max_health) curr_health = max_health;
}

void Entity::set_team(char team){
    this->team = team;
}

void Entity::take_damage(int value){
    
    int new_health = get_curr_health() - value;

    if(new_health < 0) set_curr_health(0);
    else set_curr_health(new_health);
}

void Entity::take_heal(int value){

    int new_health = get_curr_health() + value;

    if(new_health > get_max_health()) set_curr_health(get_max_health());

    set_curr_health(new_health);
}