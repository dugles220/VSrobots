#include "Item.h"

Item::Item(std::string name) : name(name) {}

std::string Item::get_name() const { return name; }
bool Item::is_used() { return used; }
void Item::set_used(bool value) { used = value; }