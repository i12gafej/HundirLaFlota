//
//

#include "game.h"

Game::Game(Player player1, Player player2){
    player1_ = player1;
    player2_ = player2;
    running_= true;
    turn_player1_ = false;
    //turn_player2_ = false;
}

bool Game::start(){
    player1_.set_board();
    player2_.set_board();

    while(this->is_running()){          //must be prepared for requests depending on turns
        turn_player1_ = true;
        //turn_player2_ = false;


        turn_player1_ = false;

    }

    return true; //when the game has finished (so the server and the client will know)
}

char* Game::attack(bool player_turn, int coord_x, int coord_y){
    

}