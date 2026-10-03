#pragma once
#ifndef FIELD_H
#define FIELD_H

#include "Cell.h"
#include "Robot.h"
#include <vector>
#include <memory>

#define MINIMAL_SIZE 3
#define MAX_SIZE 20

class Field{
public:
    Field(size_t height, size_t width);
    ~Field() = default;

    size_t get_width() const;
    size_t get_height() const;

    const Cell& get_cell(size_t x, size_t y) const;
    Cell& get_cell(size_t x, size_t y);

    std::shared_ptr<Robot> spawn_robot(size_t x, size_t y, int health, int energy, int damage, char team);
    bool move_robot(std::shared_ptr<Robot> robot, size_t x, size_t y);
    void remove_robot(std::shared_ptr<Robot> robot);

    bool are_coord_valid(size_t x, size_t y) const;

    // void extend_height(int value);
    // void extend_width(int value);

    

private:

    size_t height = 0;
    size_t width = 0;
    std::vector<std::vector<Cell>> grid;
    std::vector<std::shared_ptr<Robot>> entities;

};

#endif