#include "OverloadStatus.h"

OverloadStatus::OverloadStatus() : is_overloaded(true){}

void OverloadStatus::apply(std::shared_ptr<Robot>){}
void OverloadStatus::execute_on_turn_start(std::shared_ptr<Robot>){}
void OverloadStatus::execute_on_turn_end(std::shared_ptr<Robot>){}
bool OverloadStatus::is_expired() const { return !is_overloaded; }

void OverloadStatus::stack(std::vector<std::unique_ptr<Status>> statuses){

    for(int i = static_cast<int>(statuses.size() - 1); i >= 0; i--){
        OverloadStatus* s = dynamic_cast<OverloadStatus*>(statuses[i].get());

        if(s != nullptr && s != this){
            statuses.erase(statuses.begin() + i);
        }
    }
}
