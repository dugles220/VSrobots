#pragma once

#include "Field.h"
#include "InputMode.h"
#include "EventLog.h"

class Renderer{
public:

    virtual void render(InputMode mode, const EventLog& log) = 0;
    virtual ~Renderer() = default;

};