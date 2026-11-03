#pragma once

#include <vector>

#include "Ability.h"

#include "AOEAttackAbility.h"
#include "HealAbility.h"
#include "RangeAttackAbility.h"
#include "TeleportAbility.h"

class AbilitiesCatalog {
private:
    std::vector<std::unique_ptr<Ability>> catalog;

public:
    AbilitiesCatalog();
    const std::vector<std::unique_ptr<Ability>>& get_catalog();
    const std::vector<std::unique_ptr<Ability>>& get_catalog() const;
    void update_catalog();

};