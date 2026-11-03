#include "AbilitiesCatalog.h"

AbilitiesCatalog::AbilitiesCatalog(){

    catalog = std::vector<std::unique_ptr<Ability>>();

    catalog.push_back(std::make_unique<AOEAttackAbility>());
    catalog.push_back(std::make_unique<HealAbility>());
    catalog.push_back(std::make_unique<RangeAttackAbility>());
    catalog.push_back(std::make_unique<TeleportAbility>());

}

const std::vector<std::unique_ptr<Ability>>& AbilitiesCatalog::get_catalog(){ return catalog; }
const std::vector<std::unique_ptr<Ability>>& AbilitiesCatalog::get_catalog() const { return catalog; }

void AbilitiesCatalog::update_catalog(){
    for(int i = static_cast<int>(catalog.size() - 1); i >= 0; i--){
        Ability* a = dynamic_cast<Ability*>(catalog[i].get());

        if(a == nullptr){
            catalog.erase(catalog.begin() + i);
        }
    }
}