//
//

#include "client.h"
#include <ctime>
#include <iostream>

Client::Client(int id, std::string password){
    id_ = id;
    password_ = password;
    waiting = true;

    std::vector<std::vector<std::string>> aux (10, std::vector<std::string>(10, "A"));      //init the board
    board_ = aux;
}

bool Client::connect(){

    return true;        //placeholder
}

void Client::print_board(){
    for(int i = 0; i < 10; i++){
        for(int j = 0; j < 10; j++){
            std::cout << board_[i][j]; 
        }
    }
}

bool Client::set_board(){

    srand(time(0));
    
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

bool Client::set_to_up(int size, int x, int y){
    int boundary = x + size;
    if(boundary > 10){
        return false;
    }
    for(int i=0; i<size; i++){
        board_[x+i][y] = "B";
    }
    return true;
}

bool Client::set_to_right(int size, int x, int y){
    int boundary = y + size;
    if(boundary > 10){
        return false;
    }
    for(int i=0; i<size; i++){
        board_[x][y+i] = "B";
    }
    return true;
}

bool Client::set_to_down(int size, int x, int y){
    int boundary = x - size;
    if(boundary < 0){
        return false;
    }
    for(int i=0; i<size; i++){
        board_[x-i][y] = "B";
    }
    return true;
}

bool Client::set_to_left(int size, int x, int y){
    int boundary = y - size;
    if(boundary < 0){
        return false;
    }
    for(int i=0; i<size; i++){
        board_[x][y-i] = "B";
    }
    return true;
}