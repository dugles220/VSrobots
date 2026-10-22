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

std::shared_ptr<Robot> Field::spawn_robot(size_t x, size_t y,
    int health, int energy, int damage, int speed = 1, char team = '0'){

    if(!are_coord_valid(x, y)){
        throw std::invalid_argument(std::format(
            "Error: ({}, {}) are out of field", x, y));
    }

    auto new_robot = std::make_shared<Robot>(health, energy, damage, speed, team);

    entities.push_back(new_robot);

    get_cell(x, y).set_entity(new_robot);

    return new_robot;
}

std::shared_ptr<Factory> Field::spawn_factory(size_t x, size_t y, 
    int health, int performance, char team){

    if(!are_coord_valid(x, y) || !are_coord_valid(x + 1, y + 1)){
        throw std::invalid_argument(std::format(
            "Error: Cannot spawn factory on ({}, {})", x, y));
    }

    auto new_factory = std::make_shared<Factory>(health, performance, team);

    entities.push_back(new_factory);

    for(size_t i = y; i < y + 2; i++){
        for(size_t j = x; j < x + 2; j++){
            get_cell(i, j).set_entity(new_factory); 
        }
    }

    return new_factory;
}

void Field::delete_entity(std::shared_ptr<Entity> entity){

    size_t x, y;

    if(get_entity_position(entity, x, y)){

        if(std::dynamic_pointer_cast<Factory>(entity)){

            for(size_t i = y; i < y + 2; i++){
                for(size_t j = x; j < x + 2; j++){
                    get_cell(j, i).set_entity(nullptr);
                }
            }
        } else get_cell(x, y).set_entity(nullptr);

        auto it = std::find(entities.begin(), entities.end(), entity);

        if(it != entities.end()){
            entities.erase(it);
        }

    }
}

bool Field::move_robot(std::shared_ptr<Robot> robot, size_t new_x, size_t new_y){

    if(!are_coord_valid(new_x, new_y)){
        return false;
    }

    Cell& cell = get_cell(new_x, new_y);

    if(cell.is_occupied()){
        if(robot->decrease_energy(ATTACK_COST)){
            robot->interact(cell.get_entity());
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

std::shared_ptr<Entity> Field::get_player() const{
    for(size_t y = 0; y < get_height(); y++){
        for(size_t x = 0; x < get_width(); x++){

            std::shared_ptr<Entity> curr = grid[y][x].get_entity();

            if(curr != nullptr && curr->get_team() == PLAYER_TEAM){
                return curr;
            }
        }
    }
    return nullptr;
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

std::vector<std::shared_ptr<Entity>>& Field::get_entities(){
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

    // сложнопроходимые клетки
    grid[6][2].set_pass_difficulty(3);
    grid[7][2].set_pass_difficulty(3);
    grid[6][3].set_pass_difficulty(3);
    grid[5][2].set_pass_difficulty(2);
    grid[5][3].set_pass_difficulty(2);

    //враги
    spawn_robot(1, 1, 50, 30, 5, 1, 'E');
    spawn_robot(6, 9, 60, 40, 5, 2, 'E');
    spawn_robot(1, 9, 30, 50, 5, 3, 'E');
    spawn_robot(9, 3, 10, 40, 20, 1, 'E');

    spawn_factory(7, 2, 200, 8, 'E');
}