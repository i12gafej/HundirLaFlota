//
//

#include "board.h"
#include <iostream>

Board::Board(){
    std::vector<std::vector<std::string>> aux (10, std::vector<std::string>(10, "B"));      //init the board
    
    board_ = aux;
    n_boats_ = 4;   //or whatever the number is
    ready_ = false;
}


void Board::print_board(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            std::cout << board_[i][j];
            
            
        }

    }
}
