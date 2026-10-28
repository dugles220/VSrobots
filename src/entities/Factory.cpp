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