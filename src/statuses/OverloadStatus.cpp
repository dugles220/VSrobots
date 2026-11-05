#include "OverloadStatus.h"

OverloadStatus::OverloadStatus() : 
    Status(OVERLOAD_STATUS_NAME),
    is_overloaded(true){}

bool OverloadStatus::is_expired() const { return !is_overloaded; }

bool OverloadStatus::blocks_ability() {
    if(!is_expired()){
        is_overloaded = false;
        return true;
    } else return false;
}

void OverloadStatus::stack(std::vector<std::unique_ptr<Status>> statuses){

    for(int i = static_cast<int>(statuses.size() - 1); i >= 0; i--){
        OverloadStatus* s = dynamic_cast<OverloadStatus*>(statuses[i].get());

        if(s != nullptr && s != this){
            statuses.erase(statuses.begin() + i);
        }
    }
}
