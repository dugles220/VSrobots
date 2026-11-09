#include "Inventory.h"

bool Inventory::is_full() const { return (int)items.size() >= capacity; }

bool Inventory::add_item(std::unique_ptr<Item> item){
    if(!is_full()){
        items.push_back(std::move(item));
        return true;
    }

    return false;
}

void Inventory::use_item(int index, Robot& robot){
    if(index < 0 || index > (int)items.size() - 1) return;

    items[index]->use(robot);

    items.erase(items.begin() + index);
}

const std::vector<std::unique_ptr<Item>>& Inventory::get_items() const { return items; }