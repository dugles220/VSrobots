#pragma once
#ifndef CELL_H
#define CELL_H

class Cell{
private:
    bool is_passable = true;

public:
    Cell() = default;
    ~Cell() = default;

    bool get_passability() const;
    void set_passability(bool value);

};

#endif