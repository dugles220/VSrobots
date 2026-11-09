#pragma once

#include "Item.h"

#include <string>
#include <memory>

class Robot;

class Item {
private:
    std::string name;
    bool used = false;

public:

    Item(std::string name);

    virtual void use(Robot& robot) = 0;

    std::string get_name() const;
    bool is_used();
    void set_used(bool value);

    virtual ~Item() = default;
};