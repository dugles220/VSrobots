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
            get_cell(j, i).set_entity(new_factory); 
        }
    }

    return new_factory;
}

void Field::delete_entity(std::shared_ptr<Entity> entity){

    size_t x, y;

    if(get_entity_position(entity.get(), x, y)){
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

bool Field::move_robot(Robot& robot, size_t new_x, size_t new_y){

    if(!are_coord_valid(new_x, new_y)){
        return false;
    }

    Cell& cell = get_cell(new_x, new_y);

    if(cell.is_occupied()){
        robot.interact(cell.get_entity());
        return false;
    }
    else if(cell.get_passability() == false){ return false; } 
    else if(robot.decrease_energy(MOVE_COST)){

        size_t curr_x, curr_y;
        if(!get_entity_position(&robot, curr_x, curr_y)){
            throw "Error: unable to find entity\n";
        }

        Cell& curr_cell = get_cell(curr_x, curr_y);

        get_cell(new_x, new_y).set_entity(curr_cell.get_entity());
        get_cell(curr_x, curr_y).set_entity(nullptr);

        return true;
    }
    return false;
}

bool Field::move_robot(Robot& robot, Cell& cell){

    if(robot.decrease_energy(MOVE_COST)){

        size_t curr_x, curr_y;
        get_entity_position(&robot, curr_x, curr_y);

        Cell& curr_cell = get_cell(curr_x, curr_y);
        cell.set_entity(curr_cell.get_entity());
        curr_cell.set_entity(nullptr);

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

bool Field::get_entity_position(const Entity* entity, size_t& out_x, size_t& out_y) const{
    for(size_t y = 0; y < get_height(); y++){
        for(size_t x = 0; x < get_width(); x++){
            const std::shared_ptr<Entity> e = grid[y][x].get_entity();
            if(e != nullptr && e.get() == entity){
                out_x = x;
                out_y = y;
                return true;
            }
        }
    }
    return false;
}

const std::vector<std::shared_ptr<Entity>>& Field::get_entities(){
    return entities;
}

void Field::init_debug_level() {
    grid[3][2].set_passability(false);
    grid[3][3].set_passability(false);
    grid[3][4].set_passability(false);

    grid[7][6].set_passability(false);
    grid[8][6].set_passability(false);

    grid[6][2].set_pass_difficulty(2);
    grid[6][3].set_pass_difficulty(3);
    grid[6][4].set_pass_difficulty(2);

    grid[3][7].set_pass_difficulty(3);
    grid[4][7].set_pass_difficulty(2);

    spawn_robot(1, 1, 50, 30, 10, 1, 'E');
    spawn_robot(9, 2, 40, 20, 15, 2, 'E');
    spawn_robot(9, 7, 70, 40, 20, 3, 'E');

    spawn_factory(10, 4, 150, 5, 'E');
}

void Field::init_demo_level() {
    for (int y = 2; y <= 7; ++y) {
        if (y != 5) {
            grid[y][7].set_passability(false);
        }
    }

    for (int x = 15; x <= 18; ++x) {
        grid[4][x].set_passability(false);
        grid[8][x].set_passability(false);
    }

    for (int y = 4; y <= 8; ++y) {
        if (y != 6) {
            grid[y][14].set_passability(false);
            grid[y][19].set_passability(false);
        }
    }

    for (int y = 2; y <= 5; ++y) {
        grid[y][27].set_passability(false);
    }

    for (int x = 29; x <= 33; ++x) {
        grid[9][x].set_passability(false);
    }

    for (int y = 12; y <= 14; ++y) {
        grid[y][30].set_passability(false);
    }

    for (int x = 4; x <= 9; ++x) {
        grid[15][x].set_passability(false);
    }

    for (int y = 15; y <= 18; ++y) {
        grid[y][9].set_passability(false);
    }

    for (int x = 2; x <= 5; ++x) {
        for (int y = 9; y <= 12; ++y) {
            grid[y][x].set_pass_difficulty(2);
        }
    }

    grid[10][3].set_pass_difficulty(3);
    grid[10][4].set_pass_difficulty(3);
    grid[11][4].set_pass_difficulty(3);

    for (int x = 12; x <= 17; ++x) {
        for (int y = 11; y <= 14; ++y) {
            grid[y][x].set_pass_difficulty(2);
        }
    }

    for (int x = 13; x <= 16; ++x) {
        grid[12][x].set_pass_difficulty(3);
    }

    for (int x = 34; x <= 37; ++x) {
        for (int y = 3; y <= 7; ++y) {
            grid[y][x].set_pass_difficulty(2);
        }
    }

    grid[5][35].set_pass_difficulty(3);
    grid[5][36].set_pass_difficulty(3);
    grid[6][36].set_pass_difficulty(3);

    // враги - быстрые и слабые
    spawn_robot(3, 2, 35, 25, 10, 3, 'E');
    spawn_robot(5, 5, 40, 30, 12, 2, 'E');

    // враги -  сбалансированные
    spawn_robot(11, 2, 60, 40, 15, 2, 'E');
    spawn_robot(12, 6, 70, 50, 20, 2, 'E');
    spawn_robot(22, 3, 55, 35, 18, 2, 'E');

    // враги - медленные и сильные
    spawn_robot(23, 15, 100, 60, 30, 1, 'E');
    spawn_robot(32, 16, 120, 50, 35, 1, 'E');

    // быстрые противники
    spawn_robot(36, 10, 45, 30, 12, 3, 'E');
    spawn_robot(37, 17, 50, 35, 15, 3, 'E');

    // фабрики
    spawn_factory(20, 2, 250, 4, 'E');
    spawn_factory(36, 8, 300, 6, 'E');
    spawn_factory(25, 17, 200, 3, 'E');
}