#pragma once

class Entity;
class Cell;

struct AbilityTarget { 
    Entity* entity = nullptr;
    Cell* cell = nullptr;
};