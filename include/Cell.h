#pragma once
#ifndef CELL_H
#define CELL_H

#include "Entity.h"

class Cell{
private:
    bool is_passable = true;
    Entity *entity = nullptr;

public:
    Cell() = default;
    ~Cell() = default;

    bool get_passability() const;
    void set_passability(bool value);

    bool is_occupied() const;
    Entity* get_entity() const;
    void set_entity(Entity *entity);    

};

#endif