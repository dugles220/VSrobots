#include "StrengthStatus.h"

StrengthStatus::StrengthStatus() : Status(STRENGTH_STATUS_NAME) {}

bool StrengthStatus::is_expired() const { return turns == 0; }

int StrengthStatus::change_damage(int orig_damage){

    if(is_expired()) return orig_damage;

    turns--;

    return orig_damage * coeff;

}

void StrengthStatus::increase_turns(size_t value){
    set_turns(get_turns() + value);
}

void StrengthStatus::set_turns(size_t value){
    if(value > MAX_STRENGTH_DUR) turns = MAX_STRENGTH_DUR;
    else turns = value;
}
int StrengthStatus::get_turns() const{ return turns; }


void StrengthStatus::stack(std::vector<std::unique_ptr<Status>>& statuses){
    for(int i = static_cast<int>(statuses.size() - 1); i >= 0; i--){

        StrengthStatus* s = dynamic_cast<StrengthStatus*>(statuses[i].get());

        if(s != nullptr && s != this){
            increase_turns(s->get_turns());
            statuses.erase(statuses.begin() + i);
        }
    }
}
