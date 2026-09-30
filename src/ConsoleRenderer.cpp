#include "ConsoleRenderer.h"
#include "Field.h"
#include <iostream>

void ConsoleRenderer::render(const Field& field){

    system("clear");

    for(size_t y = 0; y < field.get_height(); y++){
        for(size_t x = 0; x < field.get_width(); x++){

            const Cell& cell = field.get_cell(x, y);

            if(cell.is_occupied()){

                auto entity = cell.get_entity();

                if(const Robot* robot = dynamic_cast<Robot*>(entity)){ 
                    if(robot->get_team() == 'P'){
                        std::cout << " P ";
                    }
                    else{
                        std::cout << " E ";
                    }
                }
            }
            else{
                if(cell.get_passability() != true){
                    std::cout << " @ " ;
                }
                else{
                    std::cout << " ~ ";
                }
            }

        }
        std::cout << "\n";
    }
}