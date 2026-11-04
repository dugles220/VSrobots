#pragma once

#include "BaseAbility.h"

#define HEAL_ABILITY_NAME "Heal"
#define DEFAULT_HEAL 50
#define MAX_HEAL_VALUE 150
#define DEFAULT_HEAL_COST 30
#define HEAL_UPGRADE_SIZE 15

class HealAbility : public BaseAbility {
private:
    int heal_value;

public:
    HealAbility();
    HealAbility(int heal_value, int energy_cost);
    bool use(Robot& robot,
             TurnContext& context,
             AbilityTarget& target);
    std::unique_ptr<Ability> clone() const {
        return std::make_unique<HealAbility>(*this);
    }
    void upgrade();
};
