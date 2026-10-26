#pragma once
#ifndef OVERLOAD_STATUS_H
#define OVERLOAD_STATUS_H

#include "Status.h"

class OverloadStatus : public Status {

private:

    bool is_overloaded;

public:

    OverloadStatus();

    void apply(std::shared_ptr<Robot>);
    void execute_on_turn_start(std::shared_ptr<Robot>);
    void execute_on_turn_end(std::shared_ptr<Robot>);
    bool is_expired() const;

    void stack(std::vector<std::unique_ptr<Status>> statuses);

};

#endif