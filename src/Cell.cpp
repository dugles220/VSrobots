#include "Cell.h"

bool Cell::get_passability() const{
    return is_passable;
}

void Cell::set_passability(bool value){
    is_passable = value;
}