#include "BaseAbility.h"

BaseAbility::BaseAbility(int energy_cost, std::string name):
                energy_cost(energy_cost),
                name(name){
    if(energy_cost < 0) this->energy_cost = 0;
}

void BaseAbility::set_energy_cost(int value){
    if (value < 0) energy_cost = 0;
    energy_cost = value;
}