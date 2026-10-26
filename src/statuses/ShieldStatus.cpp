#include "ShieldStatus.h"

ShieldStatus::ShieldStatus() : damage_block(DEFAULT_DAMAGE_BLOCK) {}

ShieldStatus::ShieldStatus(size_t damage_block) : damage_block(damage_block){
    if(damage_block > MAX_DAMAGE_BLOCK) this->damage_block = MAX_DAMAGE_BLOCK;
}

void ShieldStatus::apply(std::shared_ptr<Robot>){}
void ShieldStatus::execute_on_turn_start(std::shared_ptr<Robot>){}
void ShieldStatus::execute_on_turn_end(std::shared_ptr<Robot>){}
bool ShieldStatus::is_expired() const { return is_broken; }

void ShieldStatus::stack(std::vector<std::unique_ptr<Status>> statuses){

    for(int i = static_cast<int>(statuses.size() - 1); i >= 0; i--){
        ShieldStatus* s = dynamic_cast<ShieldStatus*>(statuses[i].get());

        if(s != nullptr && s != this){
            statuses.erase(statuses.begin() + i);
        }
    }
}
