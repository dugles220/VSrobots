#pragma once

#include "Renderer.h"

class ConsoleRenderer : public Renderer{
private:
    const Field& field;

public:
    ConsoleRenderer() = default;
    ~ConsoleRenderer() = default;

    void render(InputMode mode, const EventLog& log);
};