#include "SFMLRenderer.h"
#include "Field.h"
#include "LevelSystem.h"

#include <format>
#include <iostream>

SFMLRenderer::SFMLRenderer(Field& field)
    : window(
        sf::VideoMode({
            (unsigned int)(R_PANEL_WIDTH + field.get_width() * CELL_SIZE),
            (unsigned int)(D_PANEL_HEIGHT + field.get_height() * CELL_SIZE)
        }),
        "VSrobots"
    ),
    font(sf::Font("resources/fonts/DejaVuSans.ttf")),
    field(field) {
    window.setFramerateLimit(60);
}

void SFMLRenderer::render(InputMode mode, const EventLog& log){
    window.clear(sf::Color::Black);

    render_cells();
    render_entities();
    render_ui();
    render_event_log(log);

    if(mode == InputMode::AbilityPick){
        render_ability_menu();
    } else if(mode == InputMode::AbilityUse){
        render_ability_menu();
    }

    window.display();
}

std::optional<sf::Event> SFMLRenderer::poll_event(){ return window.pollEvent(); }

void SFMLRenderer::render_ability_menu(){

    sf::Text title(font, "Choose your ability: ", 28);
    title.setPosition({100.f, 100.f});

    sf::Text options(font,
        "1 - Heal\n"
        "2 - AOE Attack\n"
        "3 - Teleport\n"
        "4 - Distance Attack",
        22
    );

    options.setPosition({100.f, 170.f});

    window.draw(title);
    window.draw(options);
}

void SFMLRenderer::close(){
    window.close();
}

bool SFMLRenderer::is_open() const{ return window.isOpen(); }

void SFMLRenderer::render_cells(){

    int field_height = (int)field.get_height();
    int field_width = (int)field.get_width();
    int cell_size = CELL_SIZE;

    for (int y = 0; y < field_height; ++y) {
        for (int x = 0; x < field_width; ++x) {

            Cell& c = field.get_cell(x, y);

            sf::RectangleShape cell({
                static_cast<float>(cell_size),
                static_cast<float>(cell_size)
            });

            cell.setPosition({
                static_cast<float>(x * cell_size),
                static_cast<float>(y * cell_size)
            });

            if(!c.get_passability()){
                    cell.setFillColor(sf::Color(50, 50, 50));
            } else{
                int p = c.get_passability();

                switch(p){
                    case 1:
                        cell.setFillColor(sf::Color(192, 192, 192));
                        break;
                    case 2:
                        cell.setFillColor(sf::Color(255, 255, 51));
                        break;
                    case 3:
                        cell.setFillColor(sf::Color(255, 51, 51));
                        break;
                }
            }

            cell.setOutlineColor(sf::Color(100, 100, 100));
            cell.setOutlineThickness(1.f);

            window.draw(cell);

            if(c.get_statuses().size() != 0){

                sf::Vector2f pos = cell.getPosition();

                pos.x += 3;
                pos.y += 3;

                for(const auto& s : c.get_statuses()){

                    sf::RectangleShape status({5.f, 5.f});
                    status.setPosition(pos);

                    pos.x += 7;

                    if(dynamic_cast<BurnStatus*>(s.get())){
                        status.setFillColor(sf::Color(255, 0, 0));
                    }
                    else if(dynamic_cast<OverloadStatus*>(s.get())){
                        status.setFillColor(sf::Color(0, 0, 255));
                    }
                    else if(dynamic_cast<SlowdownStatus*>(s.get())){
                        status.setFillColor(sf::Color(64, 64, 64));
                    }
                    
                    window.draw(status);
                    
                }
            }
        }
    }
}

void SFMLRenderer::render_entities(){

    for(const auto& e : field.get_entities()){
            
        size_t ex, ey;
        field.get_entity_pos(e.get(), ex, ey);

        sf::Vector2f pos = {(float)(ex * CELL_SIZE + 2), (float)(ey * CELL_SIZE + 2)};

        if(const auto& r = std::dynamic_pointer_cast<Robot>(e)){

            sf::RectangleShape r_shape({28.f, 28.f});
            
            if(r->get_team() == PLAYER_TEAM){
                r_shape.setFillColor(sf::Color::Green);
            } else{
                r_shape.setFillColor(sf::Color::Red);
            }

            r_shape.setPosition(pos);
            window.draw(r_shape);

        } else if(const auto& f = std::dynamic_pointer_cast<Factory>(e)){

            sf::RectangleShape f_shape({28.f * 2, 28.f * 2});

            f_shape.setFillColor(sf::Color::Yellow);

            f_shape.setPosition(pos);
            window.draw(f_shape);
        }

        float w_max = 32.f;
        float w_curr = w_max * ((float)e->get_curr_health() / (float)e->get_max_health());
        float healthbar_height = 6.f;

        sf::RectangleShape full({w_max, healthbar_height});
        sf::RectangleShape curr({w_curr, healthbar_height});

        full.setFillColor(sf::Color::Black);
        curr.setFillColor(sf::Color::Red);

        pos.x -= 2;
        pos.y -= 8;

        full.setPosition(pos);
        curr.setPosition(pos);

        window.draw(full);
        window.draw(curr);
    }
}

void SFMLRenderer::render_ui(){

    size_t w = field.get_width();
    size_t h = field.get_height();

    sf::Vector2f rp_pos = {(float)w * CELL_SIZE, 0.f};

    float rp_w = R_PANEL_WIDTH;
    float rp_h = (float)h * (float)CELL_SIZE + D_PANEL_HEIGHT;

    sf::RectangleShape panel({rp_w, rp_h});
    panel.setFillColor(UIColors::PANEL);

    panel.setPosition(rp_pos);
    window.draw(panel);

    const auto& player = std::dynamic_pointer_cast<Robot>(field.get_player());



    sf::Text header(font, "PLAYER", 24);
    header.setFillColor(UIColors::TEXT);
    
    sf::Vector2f h_pos = rp_pos;
    sf::Vector2f h_size = header.getLocalBounds().size;

    h_pos.x = h_pos.x + (rp_w - h_size.x) / 2;

    header.setPosition(h_pos);

    window.draw(header);



    sf::Text health(font, std::format("Health: {} / {}", 
        player->get_curr_health(), player->get_max_health()), 16);
    health.setFillColor(UIColors::TEXT);
    sf::Vector2f hp_pos = rp_pos;
    hp_pos.y += h_size.y + 10;
    hp_pos.x += 10;
    
    health.setPosition(hp_pos);
    window.draw(health);



    sf::Text energy(font, std::format("Energy: {} / {}", 
        player->get_curr_energy(), player->get_max_energy()), 16);
    energy.setFillColor(UIColors::TEXT);
    sf::Vector2f en_pos = hp_pos;
    en_pos.y += health.getLocalBounds().size.y + 10;
    
    energy.setPosition(en_pos);
    window.draw(energy);



    sf::Text damage(font, std::format("Damage: {}", 
        player->get_damage()), 16);
    damage.setFillColor(UIColors::TEXT);
    sf::Vector2f d_pos = en_pos;
    d_pos.y += energy.getLocalBounds().size.y + 10;
    
    damage.setPosition(d_pos);
    window.draw(damage);



    sf::Text level(font, std::format("Level: {}    XP: {} / {}", 
        player->get_level(),
        player->get_exp(),
        LevelSystem::get_required_exp(player->get_level())),
        16);
    level.setFillColor(UIColors::TEXT);
    sf::Vector2f l_pos = d_pos;
    l_pos.y += damage.getLocalBounds().size.y + 10;
    
    level.setPosition(l_pos);
    window.draw(level);



    sf::Text abilities(font, "ABILITIES", 24);
    abilities.setFillColor(UIColors::TEXT);
    sf::Vector2f ab_pos = l_pos;
    ab_pos.y += level.getLocalBounds().size.y + 30;
    ab_pos.x = rp_pos.x + (rp_w - abilities.getLocalBounds().size.x) / 2;

    abilities.setPosition(ab_pos);
    window.draw(abilities);

    ab_pos.x = rp_pos.x + 10;
    ab_pos.y += abilities.getLocalBounds().size.y + 10;

    for(const auto& ability : player->get_abilities()){

        sf::Text a(font, ability->get_name(), 16);
        a.setFillColor(UIColors::TEXT);
        a.setPosition(ab_pos);
        window.draw(a);

        ab_pos.y += a.getLocalBounds().size.y + 10;

    }



    sf::Text statuses(font, "STATUSES", 24);
    statuses.setFillColor(UIColors::TEXT);
    sf::Vector2f st_pos = ab_pos;
    st_pos.y += 20;
    st_pos.x = rp_pos.x + (rp_w - statuses.getLocalBounds().size.x) / 2;

    statuses.setPosition(st_pos);
    window.draw(statuses);

    st_pos.x = rp_pos.x + 10;
    st_pos.y += statuses.getLocalBounds().size.y + 10;

    for(const auto& status: player->get_statuses()){

        int turns = status->get_turns();
        std::string turn_info = "";

        if(turns != -1){
            turn_info = std::format(": {} turns", turns);
        }

        sf::Text s(font, std::format("{}{}", status->get_name(), turn_info), 16);
        s.setPosition(st_pos);
        window.draw(s);

        st_pos.y += s.getLocalBounds().size.y + 10;

    }
}

void SFMLRenderer::render_event_log(const EventLog& event_log){

    float panel_y = static_cast<float>(field.get_height() * CELL_SIZE);
    float panel_x = 0.f;

    float panel_width = static_cast<float>(field.get_width() * CELL_SIZE);
    float panel_height = D_PANEL_HEIGHT;

    sf::RectangleShape panel({panel_width, panel_height});
    panel.setFillColor(UIColors::PANEL);
    panel.setPosition({panel_x, panel_y});
    window.draw(panel);

    sf::Text header(font, "EVENT LOG", 20);
    header.setFillColor(UIColors::TEXT);
    header.setPosition({panel_x + 10.f, panel_y + 5.f});
    window.draw(header);

    const auto& events = event_log.get_events();

    size_t max_visible = 5;
    float line_height = 22.f;

    size_t count = std::min(events.size(), max_visible);

    size_t start = events.size() - count;

    float text_y = panel_y + 35.f;

    for(size_t i = start; i < events.size(); ++i){

        sf::Text text(font, events[i], 14);
        text.setFillColor(UIColors::TEXT);
        text.setPosition({panel_x + 10.f, text_y});

        window.draw(text);

        text_y += line_height;
    }
}