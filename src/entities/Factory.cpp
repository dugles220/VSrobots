#include "Factory.h"
#include "AIController.h"

#include <stdexcept>

Factory::Factory(int health, int performance, char team):
    Entity(health, team), performance(performance){

    if(performance <= 0){
        throw std::invalid_argument("Error! Performance value must be greater than zero!");
    }

}

int Factory::get_performance() const{ return performance; }

void Factory::take_turn(TurnContext& context){
    context.ai.process_factory(this, context);
}

int Factory::get_exp_earn() const{

    int h_coeff = FACTORY_HEALTH_EXP_COEFF;
    int p_coeff = PERF_EXP_COEFF;

    double perf_exp = 100 / get_performance() * p_coeff;

    int earned_exp = get_max_health() * h_coeff + perf_exp;
    
    return earned_exp;
}