#include "ShieldStatus.h"

ShieldStatus::ShieldStatus() : damage_block(DEFAULT_DAMAGE_BLOCK) {}

ShieldStatus::ShieldStatus(size_t damage_block) : damage_block(damage_block){
    if(damage_block > MAX_DAMAGE_BLOCK) this->damage_block = MAX_DAMAGE_BLOCK;
}
bool ShieldStatus::is_expired() const { return is_broken; }

void ShieldStatus::stack(std::vector<std::unique_ptr<Status>> statuses){

    for(int i = static_cast<int>(statuses.size() - 1); i >= 0; i--){
        ShieldStatus* s = dynamic_cast<ShieldStatus*>(statuses[i].get());

        if(s != nullptr && s != this){
            statuses.erase(statuses.begin() + i);
        }
    }
}

int ShieldStatus::absorb_damage(int incoming_damage){

    if(is_broken) return incoming_damage;

    if(incoming_damage <= damage_block) return 0;
    else return incoming_damage - damage_block;

}