#include "Status.h"
#include "Item.h"

#include <memory>

#define GET_ITEM_STATUS_NAME "Get Item Status"

class GetItemStatus : public Status {
private:
    std::unique_ptr<Item> item;

public:
    GetItemStatus();
    GetItemStatus(std::unique_ptr<Item> item);

    void set_item(std::unique_ptr<Item> item);
    const std::unique_ptr<Item>& get_item() const { return item; }
    void apply(Robot& robot);
    bool is_expired() const { return true; }

    void stack(std::vector<std::unique_ptr<Status>>&) {};
};