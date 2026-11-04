#pragma once

#include "Field.h"

class ConsoleRenderer{
public:
    ~ConsoleRenderer() = default;

    void render(const Field& field);
};