#pragma once

#include "TurnContext.h"
#include "AbilityTarget.h"
#include <string>

class Robot;

class Ability {
public:
    virtual bool use(Robot& robot,
                     TurnContext& context,
                     AbilityTarget& target) = 0;
    virtual void upgrade() = 0;
    virtual std::unique_ptr<Ability> clone() const = 0;
    virtual bool is_target_required() const = 0;
    virtual bool is_cell_required() const = 0;
    virtual std::string get_name() const = 0;
    virtual ~Ability() = default;
};
