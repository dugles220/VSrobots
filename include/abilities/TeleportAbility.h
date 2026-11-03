#pragma once

#include "BaseAbility.h"

#define TELEPORT_ABILITY_NAME "Teleport"
#define MIN_TELEPORT_RADIUS 2
#define DEFAULT_TELEPORT_RADIUS 5
#define DEFAULT_TELEPORT_COST 50

class TeleportAbility : public BaseAbility {
private:
    int radius;

public:
    TeleportAbility();
    TeleportAbility(int radius, int energy_cost);
    bool use(Robot& robot,
             TurnContext& context,
             AbilityTarget& target);
    std::unique_ptr<Ability> clone() const {
        return std::make_unique<TeleportAbility>(*this);
    }
    bool is_cell_required() const;
};