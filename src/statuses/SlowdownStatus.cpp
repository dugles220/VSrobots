#include "SlowdownStatus.h"

SlowdownStatus::SlowdownStatus(): turns(DEFAULT_SLOWDOWN_DUR){}

SlowdownStatus::SlowdownStatus(size_t turns): turns(turns){
    if(turns > MAX_SLOWDOWN_DUR) this->turns = MAX_SLOWDOWN_DUR;
}

bool SlowdownStatus::is_expired() const { return turns == 0; }

void SlowdownStatus::increase_turns(size_t value){
    // проверка значения зашита в set_turns()
    set_turns(get_turns() + value);
}

void SlowdownStatus::set_turns(size_t value){
    if(value > MAX_SLOWDOWN_DUR) turns = MAX_SLOWDOWN_DUR;
    else turns = value;
}
size_t SlowdownStatus::get_turns() const{ return turns; }

void SlowdownStatus::stack(std::vector<std::unique_ptr<Status>> statuses){
    for(int i = static_cast<int>(statuses.size() - 1); i >= 0; i--){

        SlowdownStatus* s = dynamic_cast<SlowdownStatus*>(statuses[i].get());

        if(s != nullptr && s != this){
            increase_turns(s->get_turns());
            statuses.erase(statuses.begin() + i);
        }
    }
}