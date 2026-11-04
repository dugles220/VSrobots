#pragma once

#include <memory>

class Field;
class AIController;
class Robot;

struct TurnContext {
    Field& field;
    size_t turn;
    AIController &ai;
    Robot& player;
};