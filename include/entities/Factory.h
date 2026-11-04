#pragma once

#include "Entity.h"
#include "TurnContext.h"

#define FACTORY_HEALTH_EXP_COEFF 0.1
#define PERF_EXP_COEFF 3

class Factory : public Entity{

private:
    int performance;

public:
    Factory(int health, int performance, char team);

    int get_performance() const;
    void set_performance(int performance);

    int get_exp_earn() const;

    void take_turn(TurnContext& context);

    ~Factory() = default;

};