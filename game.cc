//
//

#include "game.h"

Game::Game(Player player1, Player player2){
    player1_ = player1;
    player2_ = player2;
    end_= false;
    turn_player1_ = false;
    //turn_player2_ = false;
}

bool Game::start(){
    player1_.set_board();
    player2_.set_board();

    bool end;
    std::string x, game_response;
    int y;

    do{                                 //must be prepared for requests depending on turns
        commute_player_turn();

        /*LOGIC TO RETRIEVE INFO FROM THE SERVER
            NOTE: the x and y have to be extrated from here.*/

        game_response = attack(turn_player1_, x, y);

        /*LOGIC TO SEND INFORMATION TO THE SERVER
            NOTE: it will send back the game_response info string, of may be treated here tho*/

        this->set_has_ended(ckeck_game_ended(turn_player1_));

    }while(!this->has_ended());

    return true; //when the game has finished (so the server and the client will know)
}

std::string Game::attack(bool player_turn, std::string x, int y){

    int aux_coord = coordinate_conversion_engine(x);
    std::string return_string, y_string;

    if(player_turn){                                        //player's 1 turn
        if(player2_.get_board()[aux_coord][y] == "A")
        {
            player2_.set_position(aux_coord,y,"X");
            y_string = std::to_string(y);
            return_string = "+Ok. AGUA:"+x+","+y_string+"\0";
            return return_string;
        }
        else
        {
            if(player2_.nearing_boats(aux_coord, y))        //tocado y no hundido
            {
                player2_.set_position(aux_coord,y,"X");
                y_string = std::to_string(y);
                return_string = "+Ok. TOCADO:"+x+","+y_string+"\0";
                return return_string;
            }
            else                                            //tocado y hundido
            {
                player2_.set_position(aux_coord,y,"X");
                y_string = std::to_string(y);
                return_string = "+Ok. HUNDIDO:"+x+","+y_string+"\0";
                return return_string;
            }
        }
    }else{                                                  //player's 2 turn
        if(player1_.get_board()[aux_coord][y] == "A")
        {
            player1_.set_position(aux_coord,y,"X");
            y_string = std::to_string(y);
            return_string = "+Ok. AGUA:"+x+","+y_string+"\0";
            return return_string;
        }
        else
        {
            if(player1_.nearing_boats(aux_coord, y))        //tocado y no hundido
            {
                player1_.set_position(aux_coord,y,"X");
                y_string = std::to_string(y);
                return_string = "+Ok. TOCADO:"+x+","+y_string+"\0";
                return return_string;
            }
            else                                            //tocado y hundido
            {
                player1_.set_position(aux_coord,y,"X");
                y_string = std::to_string(y);
                return_string = "+Ok. HUNDIDO:"+x+","+y_string+"\0";
                return return_string;
            }
        }
    }

}

int Game::coordinate_conversion_engine(std::string s){
    if(s == "A"){
        return 0;
    }else if(s == "B"){
        return 1;
    }else if(s == "C"){
        return 2;
    }else if(s == "D"){
        return 3;
    }else if(s == "E"){
        return 4;
    }else if(s == "F"){
        return 5;
    }else if(s == "G"){
        return 6;
    }else if(s == "H"){
        return 7;
    }else if(s == "I"){
        return 8;
    }else{
        return 9;
    }
}

bool Game::ckeck_game_ended(bool turn){
    std::vector<std::vector<std::string>> board;
    if(turn){
        board = player1_.get_board();
    }else{
        board = player2_.get_board();
    }
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            if(board[i][j] == "B"){
                return false;
            }
        }
    }
    return true;
}

void Game::commute_player_turn(){
    if(this->turn_player1_){
        set_turn_player1(false);
    }else{
        set_turn_player1(true);
    }
}