#pragma once

#include <deque>
#include <string>

class EventLog{
private:
    std::deque<std::string> events;
    size_t max_events = 100;

public:

    void add(const std::string& message, size_t turn);
    const std::deque<std::string>& get_events() const;

};