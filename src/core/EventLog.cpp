#include "EventLog.h"

#include <format>

void EventLog::add(const std::string& message, size_t turn){

    std::string t = std::format("[TURN {}]: ", turn);

    if(events.size() >= max_events){
        events.pop_front();
    }
    events.push_back(t + message);
}

const std::deque<std::string>& EventLog::get_events() const { return events; }