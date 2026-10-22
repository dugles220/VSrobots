#include "Robot.h"
#include <stdexcept>

Robot::Robot(int health, int energy, int damage, int speed, char team):
    Entity(health, team),
    damage(damage),
    speed(speed),
    max_energy(energy),
    curr_energy(energy){

    if(energy <= 0){
        throw std::invalid_argument("Energy value must be greater than 0!");
    }
    if(damage < 0){
        throw std::invalid_argument("Damage value must be positive!");
    }
}

void Robot::interact(std::shared_ptr<Entity> entity){

    if(get_team() == entity->get_team()){
        entity->take_heal(get_damage());
    } else {
        entity->take_damage(get_damage());
    }

}

void Robot::level_up(){

}

void Robot::increase_energy(int value){

    int new_energy = get_curr_energy() + value;

    if(new_energy > get_max_energy()){
        set_curr_energy(get_max_energy());
        return;
    }

    set_curr_energy(new_energy);

}

bool Robot::decrease_energy(int value){

    int new_energy = get_curr_energy() - value;

    if(new_energy < 0){
        return false;
    }

    set_curr_energy(new_energy);

    return true;
}

int Robot::get_speed() const{ return speed; }
int Robot::get_damage() const { return damage; }
int Robot::get_max_energy() const { return max_energy; }
int Robot::get_curr_energy() const { return curr_energy; }
double Robot::get_exp() const { return exp; }

void Robot::set_speed(int value){

    if(value < 0) speed = 0;
    if(value > MAX_SPEED) speed = MAX_SPEED;

    speed = value;
}

void Robot::set_damage(int value){
    if(value < 0){ damage = 0; }
    damage = value;
}

void Robot::set_max_energy(int value){
    if(value < 0){ max_energy = 0; }
    max_energy = value;
}

void Robot::set_curr_energy(int value){
    if(value < 0){ curr_energy = 0; }
    else if(value > max_energy){ curr_energy = max_energy; }
    curr_energy = value;
}

void Robot::set_exp(double value){
    if(value < 0){ exp = 0; }
    exp = value;
}

void Robot::add_exp(double value){
    if(value < 0){ return; }
    exp += value;
}

