#pragma once
#ifndef CELL_H
#define CELL_H

#include "Entity.h"
#include <memory>

class Cell{
private:
    bool is_passable = true;
    std::shared_ptr<Entity> entity = nullptr;

public:
    Cell() = default;
    ~Cell() = default;

    bool get_passability() const;
    void set_passability(bool value);

    bool is_occupied() const;
    std::shared_ptr<Entity> get_entity() const;

    void set_entity(std::shared_ptr<Entity> entity);    

};

#endif