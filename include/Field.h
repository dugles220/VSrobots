#pragma once
#ifndef FIELD_H
#define FIELD_H

#include "Cell.h"
#include "Robot.h"
#include <vector>

#define MINIMAL_SIZE 3
#define MAX_SIZE 20

class Field{
public:
    Field(int height, int width);
    ~Field();

    int get_width() const;
    int get_height() const;

    void extend_height(int value);
    void extend_width(int value);

    void add_robot();
    void delete_robot();
    void move_robot();

private:

    std::vector<std::vector<Cell>> grid;
    int height = 0;
    int width = 0;
    std::vector<Robot> robots;

};

#endif FIELD_H