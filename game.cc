//
//

#include "game.h"
#include "functionality.h"
#include <errno.h>
#include <string.h>
#include <cstdlib>

Game::Game(Player player1, Player player2){
    player1_ = player1;
    player2_ = player2;
    end_= false;
    turn_player1_ = false;
}

/*TODO control the contrary board*/

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
    char** info;

    do{                                 //must be prepared for requests depending on turns
        commute_player_turn();
        fd_set readfs;
        FD_ZERO(&readfs);

        do{
            bzero(buff, sizeof(buff));
            if(turn_player1_)
            {
                FD_SET(player1_.get_socket(), &readfs);
                select(FD_SETSIZE,&readfs,NULL,NULL,NULL);

                if(FD_ISSET(player1_.get_socket(), &readfs))
                {
                    if(recv(player1_.get_socket(), buff, sizeof(buff), 0) < 0){
                        printf("ERROR en la escucha del jugador 1\n%d: %s\n", errno, strerror(errno));
                        exit(EXIT_FAILURE);
                    }
                }else{
                    printf("ERROR en la recogida de info\n%d: %s\n", errno, strerror(errno));
                }
            }
            else
            {
                FD_SET(player2_.get_socket(), &readfs);
                printf("Esperando en el select del jugador dos\n");
                select(FD_SETSIZE,&readfs,NULL,NULL,NULL);

                if(FD_ISSET(player1_.get_socket(), &readfs))
                {
                    if(recv(player2_.get_socket(), buff, sizeof(buff), 0) < 0){
                        printf("ERROR en la escucha del jugador 2\n%d: %s\n", errno, strerror(errno));
                        exit(EXIT_FAILURE);
                    }
                }else{
                    printf("ERROR en la recogida de info\n%d: %s\n", errno, strerror(errno));
                }
            }

            info = split(buff, ' ');
            printf("info 1: %s, info 2: %s, info 3: %s\n", info[0], info[1], info[2]);
            if(strcmp(info[0], "SALIR") == 0){
                return !turn_player1_;
            }

        }while(strcmp(info[0], "ATAQUE") != 0);

        if(turn_player1_)
        {
            player1_.shoot();
        }
        else
        {
            player2_.shoot();
        }

        /*Problem is now in coordinates, but should be fine by inspecting them in attack function*/
        //const char*           //give it an eye
        game_response = attack(turn_player1_, info[1], info[2]);

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

        player1_.print_contrary_board();
        player2_.print_contrary_board();
        this->set_has_ended(ckeck_game_ended(turn_player1_));

    }while(!this->has_ended());

    return turn_player1_;
}

const char* Game::attack(bool player_turn, char* x, char* y){

    int x_aux = coordinate_conversion_engine(x);
    int y_aux = atoi(y);
    y_aux--;                        //para ajustar la segunda coordenada a los índices de la matriz

    std::string return_string, y_string, x_string;
    const char* aux;

    if(player_turn)
    {                                        //player's 1 turn
        if(player2_.get_board()[x_aux][y_aux] == 'A' || player2_.get_board()[x_aux][y_aux] == 'X')
        {
            player2_.set_position(x_aux,y_aux,'X');
            x_string = std::to_string(x_aux);
            y_string = std::to_string(y_aux);
            return_string = "+Ok. Agua:" + x_string + "," + y_string;
            aux = return_string.c_str();
            return aux;
        }
        else
        {
            player2_.set_position(x_aux,y_aux,'X');
            x_string = std::to_string(x_aux);
            y_string = std::to_string(y_aux);

            if(player2_.nearing_boats(x_aux, y_aux))        //tocado y no hundido
            {
                return_string = "+Ok. Tocado:"+x_string+","+y_string;
                aux = return_string.c_str();
                return aux;
            }
            else                                            //tocado y hundido
            {
                return_string = "+Ok. Hundido:"+x_string+","+y_string;
                aux = return_string.c_str();
                return aux;
            }
        }
    }
    else
    {                                                  //player's 2 turn
        if(player1_.get_board()[x_aux][y_aux] == 'A' || player1_.get_board()[x_aux][y_aux] == 'X')
        {
            player1_.set_position(x_aux,y_aux,'X');
            x_string = std::to_string(x_aux);
            y_string = std::to_string(y_aux);
            return_string = "+Ok. Agua:"+x_string+","+y_string;
            aux = return_string.c_str();
            return aux;
        }
        else
        {
            player1_.set_position(x_aux,y_aux,'X');
            x_string = std::to_string(x_aux);
            y_string = std::to_string(y_aux);

            if(player1_.nearing_boats(x_aux, y_aux))        //tocado y no hundido
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

int Game::coordinate_conversion_engine(char* s){
    if(s == "A" || s == "a"){
        return 0;
    }else if(s == "B" || s == "b"){
        return 1;
    }else if(s == "C" || s == "c"){
        return 2;
    }else if(s == "D" || s == "d"){
        return 3;
    }else if(s == "E" || s == "e"){
        return 4;
    }else if(s == "F" || s == "f"){
        return 5;
    }else if(s == "G" || s == "g"){
        return 6;
    }else if(s == "H" || s == "h"){
        return 7;
    }else if(s == "I" || s == "i"){
        return 8;
    }else{
        return 9;
    }/*else{
        return -1;
    }*/
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