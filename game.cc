//
//

#include "game.h"
#include <errno.h>
#include <string.h>
#include <cstdlib>

Game::Game(Player player1, Player player2){
    player1_ = player1;
    player2_ = player2;
    end_= false;
    turn_player1_ = false;
}

bool Game::start(){
    player1_.set_board();
    player1_.print_board();
    player2_.set_board();
    player2_.print_board();

    char buff[100];

    bool end;
    char x;
    std::string game_response;
    int y;

    do{                                 //must be prepared for requests depending on turns
        commute_player_turn();
        fd_set readfs;
        FD_ZERO(&readfs);

        bzero(buff, sizeof(buff));
        if(turn_player1_){
            FD_SET(player1_.get_socket(), &readfs);

            /*FALTA EL SELECT!!!!!*/

            if(FD_ISSET(player1_.get_socket(), &readfs))
            {
                if(recv(player1_.get_socket(), buff, sizeof(buff), 0) < 0){
                    printf("ERROR en la escucha del jugador 1\n%d: %s\n", errno, strerror(errno));
                    exit(EXIT_FAILURE);
                }
                player1_.shoot();
            }else{
                printf("ERROR en la recogida de info\n%d: %s\n", errno, strerror(errno));
            }
        }
        else
        {
            FD_SET(player2_.get_socket(), &readfs);
            if(FD_ISSET(player1_.get_socket(), &readfs))
            {
                if(recv(player2_.get_socket(), buff, sizeof(buff), 0) < 0){
                    printf("ERROR en la escucha del jugador 2\n%d: %s\n", errno, strerror(errno));
                    exit(EXIT_FAILURE);
                }
                player2_.shoot();
            }else{
                printf("ERROR en la recogida de info\n%d: %s\n", errno, strerror(errno));
            }
        }

        /*Tratamiento de la cadena enviada*/
        game_response = attack(turn_player1_, x, y);

        if(turn_player1_){
            if(send(player1_.get_socket(), game_response.c_str(), sizeof(game_response.c_str()), 0) < 0){
                printf("ERROR en el envío del jugador 1\n%d: %s\n", errno, strerror(errno));
                exit(EXIT_FAILURE);
            }
        }
        else
        {
            if(send(player2_.get_socket(), game_response.c_str(), sizeof(game_response.c_str()), 0) < 0){
                printf("ERROR en el envío del jugador 2\n%d: %s\n", errno, strerror(errno));
                exit(EXIT_FAILURE);
            }
        }

        this->set_has_ended(ckeck_game_ended(turn_player1_));

    }while(!this->has_ended());

    /*Returns the turn of the player that has won. That means true for player 1 victory and
    and false for player 2 victory. The printing can be done in the server, but also here lol*/
    return turn_player1_;
}

const char* Game::attack(bool player_turn, char x, int y){

    int aux_coord = coordinate_conversion_engine(x);
    std::string return_string, y_string, x_string;
    const char* aux;

    if(player_turn){                                        //player's 1 turn
        if(player2_.get_board()[aux_coord][y] == 'A' || player2_.get_board()[aux_coord][y] == 'X')
        {
            player2_.set_position(aux_coord,y,'X');
            y_string = std::to_string(y);
            x_string = std::to_string(x);
            return_string = "+Ok. AGUA:" + x_string + "," + y_string;
            aux = return_string.c_str();
            return aux;
        }
        else
        {
            player2_.set_position(aux_coord,y,'X');
            y_string = std::to_string(y);
            x_string = std::to_string(x);

            if(player2_.nearing_boats(aux_coord, y))        //tocado y no hundido
            {
                return_string = "+Ok. TOCADO:"+x_string+","+y_string;
                aux = return_string.c_str();
                return aux;
            }
            else                                            //tocado y hundido
            {
                return_string = "+Ok. HUNDIDO:"+x_string+","+y_string;
                aux = return_string.c_str();
                return aux;
            }
        }
    }
    else
    {                                                  //player's 2 turn
        if(player1_.get_board()[aux_coord][y] == 'A' || player1_.get_board()[aux_coord][y] == 'X')
        {
            player1_.set_position(aux_coord,y,'X');
            y_string = std::to_string(y);
            x_string = std::to_string(x);
            return_string = "+Ok. AGUA:"+x_string+","+y_string;
            aux = return_string.c_str();
            return aux;
        }
        else
        {
            player1_.set_position(aux_coord,y,'X');
            x_string = std::to_string(x);
            y_string = std::to_string(y);

            if(player1_.nearing_boats(aux_coord, y))        //tocado y no hundido
            {
                return_string = "+Ok. TOCADO:"+x_string+","+y_string+"\0";
                aux = return_string.c_str();
                return aux;
            }
            else                                            //tocado y hundido
            {
                return_string = "+Ok. HUNDIDO:"+x_string+","+y_string;
                aux = return_string.c_str();
                return aux;
            }
        }
    }

}

int Game::coordinate_conversion_engine(char s){
    if(s == 'A'){
        return 0;
    }else if(s == 'B'){
        return 1;
    }else if(s == 'C'){
        return 2;
    }else if(s == 'D'){
        return 3;
    }else if(s == 'E'){
        return 4;
    }else if(s == 'F'){
        return 5;
    }else if(s == 'G'){
        return 6;
    }else if(s == 'H'){
        return 7;
    }else if(s == 'I'){
        return 8;
    }else{
        return 9;
    }
}

bool Game::ckeck_game_ended(bool turn){
    std::vector<std::vector<char>> board;
    if(turn){
        board = player1_.get_board();
    }else{
        board = player2_.get_board();
    }
    for(int i=0; i<10; i++){
        for(int j=0; j<10; j++){
            if(board[i][j] == 'B'){
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