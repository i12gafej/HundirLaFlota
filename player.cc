//
//

#include "player.h"
#include <ctime>
#include <iostream>
#include <cstring>

Player::Player(std::string id, std::string password, int sd){
    id_ = id;
    password_ = password;
    sd_ = sd;
    waiting = true;
    shots_= 0 ;

    std::vector<std::vector<char>> aux_1 (10, std::vector<char>(10, 'A'));      //init the board
    std::vector<std::vector<char>> aux_2 (10, std::vector<char>(10, 'X'));
    board_ = aux_1;
    contrary_board_ = aux_2;
    
}

//May want to check the board in inverse order for printeability purposes
void Player::print_board(){
    char board[200];
    std::string string;
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            string.push_back(board_[i][j]);
        }
        string.push_back('\n');
    }
    string.push_back('\n');

    bzero(board, sizeof(board));
    strcpy(board, string.c_str());
    if(send(this->get_socket(), board, sizeof(board), 0) < 0){
        printf("ERROR en el envío de la tabla\n%d: %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

void Player::print_contrary_board(){
    char board[200];
    std::string string;
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            string.push_back(contrary_board_[i][j]);
        }
        string.push_back('\n');
    }
    string.push_back('\n');

    bzero(board, sizeof(board));
    strcpy(board, string.c_str());
    if(send(this->get_socket(), board, sizeof(board), 0) < 0){
        printf("ERROR en el envío de la tabla\n%d: %s", errno, strerror(errno));
        exit(EXIT_FAILURE);
    }
}

char Player::get_position(int x, int y){
    return board_[x][y];
}

bool Player::set_board(){    
    bool valid_position;
    int pos_x, pos_y, facing_way, boat_size;

    boat_size = 4;
    do{
        pos_x = rand()%10;
        pos_y = rand()%10;
        facing_way = rand()%4;  //0,1,2,3 -> up, rigth, down, left
        
        switch(facing_way){
            case 0:
                valid_position = set_to_up(boat_size, pos_x, pos_y);
            break;
            case 1:
                valid_position = set_to_right(boat_size, pos_x, pos_y);
            break;
            case 2:
                valid_position = set_to_down(boat_size, pos_x, pos_y);
            break;
            case 3:
                valid_position = set_to_left(boat_size, pos_x, pos_y);
            break;
        }

    }while(!valid_position);

    int counter = 0;
    boat_size = 3;
    do{
        pos_x = rand()%10;
        pos_y = rand()%10;
        facing_way = rand()%4;  //0,1,2,3 -> up, rigth, down, left
        
        switch(facing_way){
            case 0:
                valid_position = set_to_up(boat_size, pos_x, pos_y);
            break;
            case 1:
                valid_position = set_to_right(boat_size, pos_x, pos_y);
            break;
            case 2:
                valid_position = set_to_down(boat_size, pos_x, pos_y);
            break;
            case 3:
                valid_position = set_to_left(boat_size, pos_x, pos_y);
            break;
        }

        if(valid_position){
            counter++;
        }

    }while(counter < 2);

    counter = 0;
    boat_size = 2;
    do{
        pos_x = rand()%10;
        pos_y = rand()%10;
        facing_way = rand()%4;  //0,1,2,3 -> up, rigth, down, left
        
        switch(facing_way){
            case 0:
                valid_position = set_to_up(boat_size, pos_x, pos_y);
            break;
            case 1:
                valid_position = set_to_right(boat_size, pos_x, pos_y);
            break;
            case 2:
                valid_position = set_to_down(boat_size, pos_x, pos_y);
            break;
            case 3:
                valid_position = set_to_left(boat_size, pos_x, pos_y);
            break;
        }

        if(valid_position){
            counter++;
        }

    }while(counter < 2);
    
    return true;
}



bool Player::set_to_up(int size, int x, int y){
    int boundary = x + size;
    if(boundary > 9){
        return false;
    }
    for(int i=0; i<size; i++){
        if(nearing_boats(x+i, y)){
            return false;
        }
    }

    for(int i=0; i<size; i++){
        board_[x+i][y] = 'B';
    }
    return true;
}

bool Player::set_to_right(int size, int x, int y){
    int boundary = y + size;
    if(boundary > 9){
        return false;
    }
    for(int i=0; i<size; i++){
        if(nearing_boats(x, y+i)){
            return false;
        }
    }
    for(int i=0; i<size; i++){
        board_[x][y+i] = 'B';
    }
    return true;
}

bool Player::set_to_down(int size, int x, int y){
    int boundary = x - size;
    if(boundary < 0){
        return false;
    }
    for(int i=0; i<size; i++){
        if(nearing_boats(x-i, y)){
            return false;
        }
    }
    for(int i=0; i<size; i++){
        board_[x-i][y] = 'B';
    }
    return true;
}

bool Player::set_to_left(int size, int x, int y){
    int boundary = y - size;
    if(boundary < 0){
        return false;
    }
    for(int i=0; i<size; i++){
        if(nearing_boats(x, y-i)){
            return false;
        }
    }
    for(int i=0; i<size; i++){
        board_[x][y-i] = 'B';
    }
    return true;
}

bool Player::nearing_boats(int x, int y){
    for(int i=-1; i<=1; i++){
        for(int j=-1; j<=1; j++){
            if((i==0 && j==0) || out_of_limits(x+i, y+i)){    //elemento actual o fuera de límites
                //condición para no hacer nada
            }else{
                if(board_[x+i][y+j] == 'B'){
                    return true;
                }
            }
        }
    }
    return false;
}

bool Player::out_of_limits(int x, int y){
    if(x > 9 || x < 0 || y > 9 || y < 0){
        return true;
    }else{
        return false;
    }
}

void Player::set_position(int x, int y, char state){
    board_[x][y] = state;
}

void Player::set_contrary_position(int x, int y, char state){
    contrary_board_[x][y] = state;
}