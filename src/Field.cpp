#include "Field.h"
#include <stdexcept>
#include <algorithm>
#include <format>

Field::Field(size_t height, size_t width): height(height), width(width), grid(height, std::vector<Cell>(width)){

    if(height < MINIMAL_SIZE || width < MINIMAL_SIZE){
        throw std::invalid_argument(std::format(
            "Error: minimal size of the field is {} by {}", 
            MINIMAL_SIZE, MINIMAL_SIZE));
    }
    else if(height > MAX_SIZE || width > MAX_SIZE){
        throw std::invalid_argument(std::format(
            "Error: maximum side size is {}", MAX_SIZE));
    }
}

size_t Field::get_height() const{ return height; }
size_t Field::get_width() const { return width; }

const Cell& Field::get_cell(size_t x, size_t y) const{

    if(!are_coord_valid(x, y)){
        throw std::invalid_argument(std::format(
            "Error: ({}, {}) are out of field", x, y));
    }

    return grid[y][x];
}

Cell& Field::get_cell(size_t x, size_t y){

    if(!are_coord_valid(x, y)){
        throw std::invalid_argument(std::format(
            "Error: ({}, {}) are out of field", x, y));
    }

    return grid[y][x];
}

std::shared_ptr<Robot> Field::spawn_robot(size_t x, size_t y, int health, int energy, int damage, char team = '0'){

    if(!are_coord_valid(x, y)){
        throw std::invalid_argument(std::format(
            "Error: ({}, {}) are out of field", x, y));
    }

    auto new_robot = std::make_shared<Robot>(health, energy, damage);

    new_robot.get()->set_team(team);

    entities.push_back(new_robot);


    get_cell(x, y).set_entity(new_robot);

    return new_robot;
}

void Field::remove_robot(std::shared_ptr<Robot> robot){

    size_t x, y;

    if(get_entity_position(robot, x, y)){
                get_cell(x, y).set_entity(nullptr);
        remove_entity_from_vec(robot);
    }
}

bool Field::move_robot(std::shared_ptr<Robot> robot, size_t new_x, size_t new_y){

    if(!are_coord_valid(new_x, new_y)){
        return false;
    }

    Cell& cell = get_cell(new_x, new_y);

    if(cell.is_occupied()){
if(robot->decrease_energy(ATTACK_COST)){
        robot->interact(std::dynamic_pointer_cast<Robot>(cell.get_entity()));
        }
        return false;
    }
    else if(!cell.get_passability()){ return false; } 
    else if(robot->decrease_energy(MOVE_COST)){

        size_t curr_x, curr_y;
        if(!get_entity_position(robot, curr_x, curr_y)){
            throw "Error: unable to find entity\n";
        }

        get_cell(curr_x, curr_y).set_entity(nullptr);
    get_cell(new_x, new_y).set_entity(robot);
        return true;
}

    return false;
}

bool Field::are_coord_valid(size_t x, size_t y) const{

    if(x >= get_width() || y >= get_height()) return false;

    return true;
}

bool Field::get_entity_position(const std::shared_ptr<Entity> entity, size_t& out_x, size_t& out_y) const{
    for(size_t y = 0; y < get_height(); y++){
        for(size_t x = 0; x < get_width(); x++){
            if(grid[y][x].get_entity() == entity){
                out_x = x;
                out_y = y;
                return true;
            }
        }
    }
    return false;
}

std::vector<std::shared_ptr<Robot>>& Field::get_entities(){
    return entities;
}

void Field::init_demo_level() {
    // стены
    grid[2][2].set_passability(false);
    grid[3][3].set_passability(false);
    grid[2][3].set_passability(false);
    grid[2][4].set_passability(false);

    grid[6][7].set_passability(false);
    grid[5][7].set_passability(false);
    grid[7][7].set_passability(false);
    grid[7][8].set_passability(false);
    grid[8][7].set_passability(false);

    //враги
    spawn_robot(1, 1, 50, 30, 5, 'E');
    spawn_robot(6, 9, 60, 40, 5, 'E');
    spawn_robot(1, 9, 30, 50, 5, 'E');
    spawn_robot(9, 3, 10, 40, 20, 'E');
}
bool Field::remove_entity_from_vec(std::shared_ptr<Robot> entity){

    auto it = std::find(entities.begin(), entities.end(), entity);

    if(it != entities.end()){
        entities.erase(it);
        return true;
    }

    return false;
}