#include "BurnStatus.h"

BurnStatus::BurnStatus(): damage(DEFAULT_BURN_DAMAGE),
                          turns(DEFAULT_BURN_DUR){}

BurnStatus::BurnStatus(size_t damage, size_t turns):
                       damage(damage),
                       turns(turns){
    if(damage > MAX_BURN_DAMAGE) damage = MAX_BURN_DAMAGE;
    if(turns > MAX_BURN_DUR) turns = MAX_BURN_DUR;
}

BurnStatus::~BurnStatus() = default;

size_t BurnStatus::get_damage() const { return damage; }
void BurnStatus::set_damage(size_t value){
    if(value > MAX_BURN_DAMAGE) damage = MAX_BURN_DAMAGE;
    else damage = value;
}

void BurnStatus::execute_on_turn_start(std::shared_ptr<Robot> robot){
    robot->take_damage(get_damage());
}

void BurnStatus::increase_turns(size_t value){
    // проверка значения зашита в set_turns()
    set_turns(get_turns() + value);
}

void BurnStatus::set_turns(size_t value){
    if(value > MAX_BURN_DUR) turns = MAX_BURN_DUR;
    else turns = value;
}

size_t BurnStatus::get_turns() const{ return turns; }

void BurnStatus::stack(std::vector<std::unique_ptr<Status>> statuses){
    for(int i = static_cast<int>(statuses.size() - 1); i >= 0; i--){
        BurnStatus* s = dynamic_cast<BurnStatus*>(statuses[i].get());

        if(s != nullptr && s != this){
            increase_turns(s->get_turns());
            statuses.erase(statuses.begin() + i);
        }
    }
}
