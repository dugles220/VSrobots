#include "Robot.h"
#include <stdexcept>

Robot::Robot(int health, int energy, int damage){

    if(health <= 0){
        throw std::invalid_argument("Health value must be greater than 0!");
    }
    if(energy <= 0){
        throw std::invalid_argument("Energy value must be greater than 0!");
    }
    if(damage <= 0){
        throw std::invalid_argument("Damage value must be greater than 0!");
    }

    max_health = health;
    curr_health = health;

    max_energy = energy;
    curr_energy = energy;

    this->damage = damage;

}


void Robot::interact(std::shared_ptr<Robot> other_robot){

    if(get_team() == other_robot->get_team()){
        other_robot->take_heal(get_damage());
    } else {
        other_robot->take_damage(get_damage());
    }

}

void Robot::level_up(){

}

void Robot::take_damage(int value){
    set_curr_health(get_curr_health() - value);
}

void Robot::take_heal(int value){
    set_curr_health(get_curr_health() + value);
}

int Robot::get_max_health() const{ return max_health; }
int Robot::get_curr_health() const { return curr_health; }
int Robot::get_damage() const { return damage; }
int Robot::get_max_energy() const { return max_energy; }
int Robot::get_curr_energy() const { return curr_energy; }
double Robot::get_exp() const { return exp; }
char Robot::get_team() const { return team; }


void Robot::set_max_health(int value){
    if(value <= 0){ max_health = 0; }
    max_health = value;
}

void Robot::set_curr_health(int value){
    if(value <= 0){ curr_health = 0; }
    else if (value > max_health){ curr_health = max_health; }
    curr_health = value;
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

void Robot::set_team(char team){
    this->team = team;
}