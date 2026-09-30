#pragma once
#ifndef CONSOLE_RENDERER_H
#define CONSOLE_RENDERER_H

#include "Field.h"

class ConsoleRenderer{
public:
    ~ConsoleRenderer() = default;

    void render(const Field& field);
};

#endif