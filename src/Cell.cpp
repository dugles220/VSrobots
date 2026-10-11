#include "Cell.h"

bool Cell::get_passability() const{ return is_passable; }

void Cell::set_passability(bool value){ is_passable = value; }

bool Cell::is_occupied() const{ return entity != nullptr; }

std::shared_ptr<Entity> Cell::get_entity() const{ return entity; }

void Cell::set_entity(std::shared_ptr<Entity> entity){ this->entity = entity; }

bool Cell::get_visibility() const{ return is_visible; }

void Cell::set_visibility(bool value){ is_visible = value; }

size_t Cell::get_pass_difficulty() const{ return pass_difficulty; }

void Cell::set_pass_difficulty(int value){

    if(value < 0) pass_difficulty = 1;
    if(value > 3) pass_difficulty = 3;

    pass_difficulty = (size_t)value;
}