#pragma once

#include "Item.h"

#include <memory>
#include <vector>

class Robot;

class Inventory {
private:
    std::vector<std::unique_ptr<Item>> items;
    int capacity = 5;

public:

    bool add_item(std::unique_ptr<Item> item);
    void use_item(int index, Robot& robot);
    bool is_full() const;
    const std::vector<std::unique_ptr<Item>>& get_items() const;

};