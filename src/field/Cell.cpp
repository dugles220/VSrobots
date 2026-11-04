#include "Cell.h"
#include "Robot.h"

#define FIRST_STATUS_PROB 40
#define SECOND_STATUS_PROB 8
#define THIRD_STATUS_PROB 3

Cell::Cell() : pass_difficulty(MIN_PASS_DIFF),
                is_passable(true),
                is_visible(false),
                entity(nullptr){
    
    for(auto& s : statuses){
        s = nullptr;
    }

}

bool Cell::get_passability() const{ return is_passable; }

void Cell::set_passability(bool value){ is_passable = value; }

bool Cell::is_occupied() const{ return entity != nullptr; }

std::shared_ptr<Entity> Cell::get_entity() const{ return entity; }

void Cell::set_entity(std::shared_ptr<Entity> entity){ this->entity = entity; }

bool Cell::get_visibility() const{ return is_visible; }

void Cell::set_visibility(bool value){ is_visible = value; }

size_t Cell::get_pass_difficulty() const{ return pass_difficulty; }

void Cell::set_pass_difficulty(int value){

    if(value < MIN_PASS_DIFF) pass_difficulty = MIN_PASS_DIFF;
    if(value > MAX_PASS_DIFF) pass_difficulty = MAX_PASS_DIFF;

    pass_difficulty = (size_t)value;
}

void Cell::generate_statuses(std::mt19937& gen){

    std::discrete_distribution<int> count_dist{FIRST_STATUS_PROB,
        SECOND_STATUS_PROB,
        THIRD_STATUS_PROB};
    
    int count = count_dist(gen);

    for(int i = 0; i < count; i++){
        statuses.push_back(generate_random_status(gen));
    }
}

std::unique_ptr<Status> Cell::generate_random_status(std::mt19937& gen){

    std::uniform_int_distribution<int> dist(0, 2);

    switch (dist(gen)) {
        case 0:
            return std::make_unique<BurnStatus>();
        case 1:
            return std::make_unique<OverloadStatus>();
        case 2:
            return std::make_unique<SlowdownStatus>();
    }

    return nullptr;
}

void Cell::transfer_status(Robot& robot, std::mt19937& gen){

    if(statuses.empty()) return;

    std::uniform_int_distribution<size_t> dist(0, statuses.size() - 1);

    size_t index = dist(gen);

    robot.add_status(std::move(statuses[index]));

    statuses.erase(statuses.begin() + index);
}