#include "ConsoleRenderer.h"
#include "Field.h"
#include <iostream>
#include <cmath>

void ConsoleRenderer::render(const Field& field){

    system("clear");

    // вычисляем координаты игрока
    size_t px, py;
    field.get_entity_position(field.get_player(), px, py);

    // проходимся по каждой клетке
    for(size_t y = 0; y < field.get_height(); y++){
        for(size_t x = 0; x < field.get_width(); x++){

            bool is_displayed = false;

            const Cell& cell = field.get_cell(x, y);

            // если клетка видна - отрисовываем
            if(cell.get_visibility() == true){
                if(cell.is_occupied()){

                    auto entity = cell.get_entity();

                    if(const std::shared_ptr<Robot> robot = 
                       std::dynamic_pointer_cast<Robot>(entity)){ 

                        // если робот на клетке - игрок, отрисовываем игрока
                        if(robot->get_team() == PLAYER_TEAM){ 
                            std::cout << " P ";
                            is_displayed = true;
                        }
                        /* если робот - враг, то проверяем, находится ли он
                           в зоне видимости игрока
                           если да - отрисовываем, если нет - отрисовываем клетку
                        */
                        else{ 
                            size_t rx, ry;
                            field.get_entity_position(robot, rx, ry);

                            int dist = std::pow(abs(px - rx), 2) + std::pow(abs(py - ry), 2);

                            if(dist <= std::pow(VISIBILITY_RADIUS, 2)){
                                std::cout << " E ";
                                is_displayed = true;
                            }
                        }
                    }
                    else if(const std::shared_ptr<Factory> f = 
                            std::dynamic_pointer_cast<Factory>(entity)){

                        std::cout << " F " ;
                        is_displayed = true;
                    }
                }

                if(!is_displayed){
                    if(cell.get_passability() == true){
                        size_t difficulty = cell.get_pass_difficulty();
                        switch (difficulty){
                            case 1:
                                std::cout << " . ";
                                break;
                            case 2:
                                std::cout << " ~ ";
                                break;
                            case 3:
                                std::cout << " ^ ";
                                break;
                            default:
                                std::cout << " ? ";
                        }
                    }
                    else{ std::cout << " # " ; }
                }
            // если клетка не видна - она не отрисовывается
            } else { std::cout << " ? "; }  
        }
        std::cout << "\n";
    }
}