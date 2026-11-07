#pragma once

#include "SFMLRenderer.h"
#include "GameCycle.h"

class Game {
private:
    GameCycle game;
    SFMLRenderer renderer;

    void process_events();
    void process_key(sf::Keyboard::Key key);
    void process_mouse(sf::Vector2i position);

    int picked_ability_index = -1;

public:

    Game();

    void run();

};