#pragma once

#include "Field.h"
#include "InputMode.h"

class Renderer{
public:

    virtual void render(InputMode mode) = 0;
    virtual ~Renderer() = default;

};