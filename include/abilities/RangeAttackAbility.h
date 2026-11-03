#pragma once

#include "BaseAbility.h"

#define RANGE_ATTACK_ABILITY_NAME "Range attack"
#define DEFAULT_RANGE_ATTACK_RADIUS 3
#define DEFAULT_RANGE_ATTACK_COST 30

class RangeAttackAbility : public BaseAbility {
private:
    int radius;

public:
    RangeAttackAbility();
    RangeAttackAbility(int radius, int energy_cost);

    bool use(Robot& robot,
             TurnContext& context,
             AbilityTarget& target);
    std::unique_ptr<Ability> clone() const {
        return std::make_unique<RangeAttackAbility>(*this);
    }
    bool is_target_required() const;

};