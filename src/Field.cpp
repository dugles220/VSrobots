#include "Field.h"
#include <stdexcept>

Field::Field(size_t height, size_t width): height(height), width(width), grid(height, std::vector<Cell>(width)){

    if(height < MINIMAL_SIZE || width < MINIMAL_SIZE){
        throw std::invalid_argument("The minimum field size is 3x3!");
    }
    else if(height > MAX_SIZE || width > MAX_SIZE){
        throw std::invalid_argument("The maximum side size is 20!");
    }

}

size_t Field::get_height() const{ return height; }
size_t Field::get_width() const { return width; }

const Cell& Field::get_cell(size_t x, size_t y) const{

    if(!are_coord_valid(x, y)){
        throw std::invalid_argument("(x,y) coordinates are out of field!");
    }

    return grid[y][x];
}

Cell& Field::get_cell(size_t x, size_t y){

    if(!are_coord_valid(x, y)){
        throw std::invalid_argument("(x,y) coordinates are out of field!");
    }

    return grid[y][x];
}

// void Field::extend_height(int value){

// }

// void Field::extend_width(int value){

// }

void Field::spawn_robot(size_t x, size_t y, int health, int energy, int damage){

    auto new_robot = std::make_shared<Robot>(health, energy, damage);

    entities.push_back(new_robot);

    get_cell(x, y).set_entity(new_robot);
}

void Field::remove_robot(std::shared_ptr<Robot> robot){

    for(size_t y = 0; y < get_height(); y++){
        for(size_t x = 0; x < get_width(); x++){
            if(get_cell(x, y).get_entity() == robot){
                get_cell(x, y).set_entity(nullptr);
                return;
            }
        }
    }
}

void Field::move_robot(std::shared_ptr<Robot> robot, size_t new_x, size_t new_y){

    if(!are_coord_valid(new_x, new_y)){
        throw std::invalid_argument("(x,y) coordinates are out of field!");
    }

    remove_robot(robot);

    get_cell(new_x, new_y).set_entity(robot);

}

bool Field::are_coord_valid(size_t x, size_t y) const{

    if(x >= get_width() || y >= get_height()) return false;

    return true;
}