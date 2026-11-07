#include "Game.h"

Game::Game() : game(20, 40),
               renderer(game.get_field()){}

void Game::process_events() {
    while (const auto event = renderer.poll_event()) {

        if (event->is<sf::Event::Closed>()) {
            renderer.close();
            continue;
        }

        if (const auto* key =
            event->getIf<sf::Event::KeyPressed>()) {

            process_key(key->code);
        }

        if (const auto* mouse =
            event->getIf<sf::Event::MouseButtonPressed>()) {

            if (mouse->button == sf::Mouse::Button::Left) {
                process_mouse(mouse->position);
            }
        }
    }
}

void Game::process_key(sf::Keyboard::Key key){

    switch(game.get_input_mode()){
        case(InputMode::AbilityPick):

            renderer.render_ability_menu();

            switch(key){
                case sf::Keyboard::Key::Num1:
                    game.process_ability_pick(0);
                    break;
                
                case sf::Keyboard::Key::Num2:
                    game.process_ability_pick(1);
                    break;

                case sf::Keyboard::Key::Num3:
                    game.process_ability_pick(2);
                    break;
                
                case sf::Keyboard::Key::Num4:
                    game.process_ability_pick(3);
                    break;
                
                default:
                    break;
            }

            break;

        case(InputMode::Normal):
            
            switch(key){
                case sf::Keyboard::Key::W:
                    game.process_input(PlayerCommand::MoveUp);
                    break;
                
                case sf::Keyboard::Key::A:
                    game.process_input(PlayerCommand::MoveLeft);
                    break;

                case sf::Keyboard::Key::S:
                    game.process_input(PlayerCommand::MoveDown);
                    break;
                
                case sf::Keyboard::Key::D:
                    game.process_input(PlayerCommand::MoveRight);
                    break;

                case sf::Keyboard::Key::B:
                    game.set_input_mode(InputMode::AbilityUse);
                    break;
                
                case sf::Keyboard::Key::Enter:
                    game.process_input(PlayerCommand::Wait);
                    break;

                case sf::Keyboard::Key::Escape:
                    renderer.close();
                
                default:
                    break; 
            }

            break;

        case(InputMode::AbilityUse):{

            switch(key){
                case sf::Keyboard::Key::Num1:
                    picked_ability_index = 0;
                    break;
                
                case sf::Keyboard::Key::Num2:
                    picked_ability_index = 1;
                    break;

                case sf::Keyboard::Key::Num3:
                    picked_ability_index = 2;
                    break;
                
                case sf::Keyboard::Key::Num4:
                    picked_ability_index = 3;
                    break;
                
                case sf::Keyboard::Key::Escape:
                    picked_ability_index = -1;
                    game.set_input_mode(InputMode::Normal);
                    break;
                
                default:
                    break;
            }

            const auto& player = std::dynamic_pointer_cast<Robot>(game.get_field().get_player());

            if(picked_ability_index < (int)player->get_abilities().size() &&
                picked_ability_index != -1){
                auto& a = player->get_abilities()[picked_ability_index];

                if(a->is_cell_required() || a->is_target_required()){
                    game.set_input_mode(InputMode::TargetPick);
                    break;
                }
            }

            if(game.process_ability_use(picked_ability_index, nullptr)){
                picked_ability_index = -1;
            }

            break;
        }
        case(InputMode::TargetPick):{
            switch(key){
                case sf::Keyboard::Key::Escape:
                    picked_ability_index = -1;
                    game.set_input_mode(InputMode::Normal);
                    break;
                default:
                    break;
            }
        }
        default:
            break;
    }
}

void Game::run(){

    while(renderer.is_open() && !game.is_game_over()) {

        process_events();

        game.update();

        renderer.render(game.get_input_mode(), game.get_log());

    }
}

void Game::process_mouse(sf::Vector2i position) {

    sf::Vector2f world_position =
        renderer.get_window().mapPixelToCoords(position);

    int x = static_cast<int>(world_position.x / CELL_SIZE);
    int y = static_cast<int>(world_position.y / CELL_SIZE);

    Field& field = game.get_field();

    if (x < 0 || y < 0 ||
        x >= static_cast<int>(field.get_width()) ||
        y >= static_cast<int>(field.get_height())) {
        return;
    }

    Cell* cell = &field.get_cell(x, y);

    switch(game.get_input_mode()){

        case(InputMode::TargetPick):
            game.process_ability_use(picked_ability_index, cell);
            break;

        default:
            break;
    }

}