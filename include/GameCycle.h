#ifndef GAME_CYCLE_H
#define GAME_CYCLE_H

#include "Field.h"
#include "ConsoleRenderer.h"

class GameCycle{

private:
    Field field;
    ConsoleRenderer renderer;

public:

    GameCycle(size_t field_height, size_t field_width);

    void run();

};

#endif