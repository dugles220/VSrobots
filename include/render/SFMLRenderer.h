#pragma once

#include "Renderer.h"
#include "SFML/Graphics.hpp"
#include "UIColors.h"

#define CELL_SIZE 32
#define R_PANEL_WIDTH 200
#define D_PANEL_HEIGHT 150

class SFMLRenderer : public Renderer{
private:
    sf::RenderWindow window;
    sf::Font font;
    Field& field;

public:
    SFMLRenderer(Field& field);

    void render(InputMode mode, const EventLog& log);

    void render_cells();
    void render_statuses(sf::Vector2f pos, Cell& cell);
    void render_entities();
    void render_ui();
    void render_ability_menu();
    void render_event_log(const EventLog& event_log);

    sf::RenderWindow& get_window() {return window; }

    void close();
    bool is_open() const;
    std::optional<sf::Event> poll_event();
};