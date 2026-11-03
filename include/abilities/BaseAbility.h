#pragma once

#include "Ability.h"
#include <string>

class BaseAbility : public Ability {
private:
    int energy_cost;
    std::string name;

public:
    BaseAbility(int energy_cost, std::string name);
    virtual ~BaseAbility() = default;

    virtual bool use(Robot& robot,
                     TurnContext& context,
                     AbilityTarget& target) = 0;
    virtual std::unique_ptr<Ability> clone() const = 0;
    virtual void upgrade() {}
    virtual bool is_target_required() const { return false; }
    virtual bool is_cell_required() const { return false; }

    int get_energy_cost() const { return energy_cost; }
    std::string get_name() const { return name; }

};