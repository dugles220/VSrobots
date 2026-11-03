#pragma once

#include "BaseAbility.h"

#define AOE_ATTACK_ABILITY_NAME "Area Attack"
#define MIN_AOE_ATTACK_RADIUS 1
#define DEFAULT_AOE_ATTACK_RADIUS 1
#define DEFAULT_AOE_ATTACK_COST 40

class AOEAttackAbility : public BaseAbility {
private:
    int radius;

public:
    AOEAttackAbility();
    AOEAttackAbility(int energy_cost, int radius);
    bool use(Robot& robot,
             TurnContext& context,
             AbilityTarget& target);
    std::unique_ptr<Ability> clone() const {
        return std::make_unique<AOEAttackAbility>(*this);
    }
};
