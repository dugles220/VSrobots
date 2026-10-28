#pragma once
#ifndef TURN_CONTEXT_H
#define TURN_CONTEXT_H

#include <memory>

class Field;
class AIController;
class Robot;

struct TurnContext {
    Field& field;
    size_t turn;
    AIController &ai;
    std::shared_ptr<Robot> player;
};

#endif