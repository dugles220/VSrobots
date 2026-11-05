#pragma once

#include "Status.h"

#define OVERLOAD_STATUS_NAME "Overload"

class OverloadStatus : public Status {

private:

    bool is_overloaded;

public:

    OverloadStatus();

    bool is_expired() const;
    bool blocks_ability();

    void stack(std::vector<std::unique_ptr<Status>> statuses);

};