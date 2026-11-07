#include "GetItemStatus.h"
#include "Robot.h"

GetItemStatus::GetItemStatus() : Status(GET_ITEM_STATUS_NAME) {}

GetItemStatus::GetItemStatus(std::unique_ptr<Item> item) :
    Status(GET_ITEM_STATUS_NAME),
    item(std::move(item)) {}

void GetItemStatus::set_item(std::unique_ptr<Item> item){
    this->item = std::move(item);
}

void GetItemStatus::apply(Robot& robot){

    if(robot.get_team() != PLAYER_TEAM) return;

    robot.get_inventory().add_item(std::move(item));

}