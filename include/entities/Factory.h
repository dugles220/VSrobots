#pragma once

#include "Entity.h"
#include "TurnContext.h"

class Factory : public Entity{

private:
    int performance;

public:

    int get_performance() const;

    void set_performance(int performance);

    Factory(int health, int performance, char team);

    void take_turn(TurnContext& context);

    ~Factory() = default;

};