#pragma once
#ifndef OVERLOAD_STATUS_H
#define OVERLOAD_STATUS_H

#include "Status.h"

class OverloadStatus : public Status {

private:

    bool is_overloaded;

public:

    OverloadStatus();

    bool is_expired() const;
    bool blocks_ability();

    void stack(std::vector<std::unique_ptr<Status>> statuses);

};

#endif