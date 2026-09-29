#include "Field.h"
#include <stdexcept>

Field::Field(int height, int width){

    if(height < MINIMAL_SIZE || width < MINIMAL_SIZE){
        throw std::invalid_argument("The minimum field size is 3x3!");
    }
    else if(height > MAX_SIZE || width > MAX_SIZE){
        throw std::invalid_argument("The maximum side size is 20!");
    }

}

int Field::get_height() const{ return height; }
int Field::get_width() const { return width; }

void Field::extend_height(int value){

}

void Field::extend_width(int value){

}

void Field::add_robot(){

}

void Field::delete_robot(){

}

void Field::move_robot(){

}