#pragma once
#ifndef CELL_H
#define CELL_H

#include "Entity.h"

#define MIN_PASS_DIFF 1
#define MAX_PASS_DIFF 3

class Cell{

private:

    size_t pass_difficulty = MIN_PASS_DIFF;
    bool is_passable = true;
    bool is_visible = false;
    std::shared_ptr<Entity> entity = nullptr;

public:

    Cell() = default;
    ~Cell() = default;

    bool get_passability() const;
    bool get_visibility() const;
    bool is_occupied() const;
    std::shared_ptr<Entity> get_entity() const;
    size_t get_pass_difficulty() const;

    void set_entity(std::shared_ptr<Entity> entity);    
    void set_passability(bool value);
    void set_visibility(bool value);
    void set_pass_difficulty(int value);

};

#endif