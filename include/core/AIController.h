#pragma once

class Field;
class Robot;
class Factory;
#include <memory>
#include "GameConstants.h"
#include "TurnContext.h"

class AIController {
private:
    Field& field;
    std::shared_ptr<Robot> player;

public:

    AIController(Field& field, std::shared_ptr<Robot> player);
    void process_robot(Robot* r, TurnContext& context);
    void process_factory(Factory* f, TurnContext& context);

    ~AIController() = default;
};