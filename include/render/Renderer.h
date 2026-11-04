#pragma once

class Field;

class Renderer{

public:
    virtual void render(const Field& field) = 0;
    virtual ~Renderer() = default;

};