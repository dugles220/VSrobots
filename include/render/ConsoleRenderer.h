#pragma once

#include "Renderer.h"

class ConsoleRenderer : public Renderer{
public:
    ~ConsoleRenderer() = default;

    void render(const Field& field);
};