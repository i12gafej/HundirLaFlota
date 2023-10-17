//
//

#include "board.h"
#include <iostream>

Board::Board(){
    initialise_board();
    n_boats_ = 4;   //or whatever the number is
    ready_ = false;
}

void Board::initialise_board(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            board_[i][j] = "B";
        }

    }
}


void Board::print_board(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            std::cout << board_[i][j];
        }

    }
}