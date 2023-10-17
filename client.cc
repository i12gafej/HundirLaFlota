//
//

#include "client.h"

Client::Client(int id, std::string password){
    id_ = id;
    password_ = password;
}

bool Client::connect(){

}

bool Client::set_board(){
    Board board = this->board_;

    while(!board.is_ready()){       //need to keep assigning boats
        

    }

    this->board_ = board;
    return true;
}