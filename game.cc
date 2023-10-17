//
//

#include "game.h"

Game::Game(Client player1, Client player2){
    player1_ = player1;
    player2_ = player2;
    running_=true;
    turn_player1_ = false;
    turn_player2_ = false;
}

void Game::start(){
    player1_.set_board();
    player2_.set_board();

    while(this->is_running()){          //must be prepared for requests depending on turns

    }

}