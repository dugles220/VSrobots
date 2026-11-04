#include "Robot.h"
#include "AIController.h"
#include "LevelSystem.h"

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

bool Robot::interact(std::shared_ptr<Entity> entity){
 
    if(get_team() != entity->get_team()){
        return attack(*entity);
    } else {
        return heal(*entity);
    }
}

bool Robot::heal(Entity& target){

    if(decrease_energy(HEAL_COST) == true){
        target.take_heal(get_damage());
        return true;
    }

    return false;
}

bool Robot::attack(Entity& target){

    if(decrease_energy(ATTACK_COST) == true){
        target.take_damage(get_damage());
        return true;
    }

    return false;
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

int Robot::get_exp() const { return experience; }
int Robot::get_level() const { return level; }

int Robot::add_exp(int amount){

    if(amount < 0){ return 0; }

    experience += amount;

    int levels_gained = 0;

    while (experience >= LevelSystem::get_required_exp(get_level())){
        experience -= LevelSystem::get_required_exp(get_level());
        level_up();
        levels_gained++;
    }

    return levels_gained;
}

void Robot::level_up(){

    level++;

    LevelBonus lb = LevelSystem::get_level_bonus(get_level());

    set_max_health(get_max_health() + lb.health);
    set_max_energy(get_max_energy() + lb.energy);
    set_damage(get_damage() + lb.damage);

}

void Robot::take_damage(int amount){

    for(auto& status : statuses){
        amount = status->absorb_damage(amount);
        if(amount <= 0) break;
    }

    if(amount > 0) Entity::take_damage(amount);
}

void Robot::update_statuses(){
    for(int i = static_cast<int>(statuses.size() - 1); i >= 0; i--){
        Status* s = statuses[i].get();

        if(s->is_expired()){
            statuses.erase(statuses.begin() + i);
        }
    }
}

void Robot::take_turn(TurnContext& context){

    if(get_team() == ENEMY_TEAM){
        context.ai.process_robot(this, context);
    }
}

const std::vector<std::unique_ptr<Status>>& Robot::get_statuses(){ return statuses; }

void Robot::add_status(std::unique_ptr<Status> status){
    statuses.push_back(std::move(status));
}

const std::vector<std::unique_ptr<Ability>>& Robot::get_abilities(){ return abilities; }

void Robot::add_ability(std::unique_ptr<Ability> ability){
    for(const auto& a : get_abilities()){
        if(a->get_name() == ability->get_name()){
            a->upgrade();
            return;
        }
    }
    abilities.push_back(std::move(ability));
}

int Robot::get_exp_earn() const{

    double h_coeff = ROBOT_HEALTH_EXP_COEFF;
    double e_coeff = ROBOT_ENERGY_EXP_COEFF;
    double d_coeff = ROBOT_DAMAGE_EXP_COEFF;

    int earned_exp = get_max_health() * h_coeff +
                     get_max_energy() * e_coeff +
                     get_damage() * d_coeff;

    return earned_exp;
}