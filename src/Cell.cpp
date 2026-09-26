#include "Cell.h"

bool Cell::get_passability() const{ return is_passable; }

void Cell::set_passability(bool value){ is_passable = value; }

bool Cell::is_occupied() const{ return entity != nullptr; }

Entity* Cell::get_entity() const{ return entity; }

void Cell::set_entity(Entity *ent){ entity = ent; }