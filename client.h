//
//

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include "board.h"

class Client{
    private:
        int id_;
        std::string password_;
        Board board_;

    public:
        //Constructor
        Client(int id, std::string password);

        //Getters
        int get_id(){return id_;}
        std::string get_password(){return password_;}
        Board get_board(){return board_;}

        //Setters
        void set_id(int id){id_ = id;}
        void set_password(std::string password){password_ = password;}

        //Methods
        bool connect();
        bool set_board();

};

#endif