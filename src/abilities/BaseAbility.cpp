#include "BaseAbility.h"

BaseAbility::BaseAbility(int energy_cost, std::string name):
                energy_cost(energy_cost),
                name(name){
    if(energy_cost < 0) this->energy_cost = 0;
}