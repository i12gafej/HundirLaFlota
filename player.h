//
//

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>

class Player{
    private:
        std::string id_;
        std::string password_;
        std::vector<std::vector<std::string>> board_;
        int sd_;
        bool waiting;


        /*Había pensado en algo así como tener un vector con los el tamaño de los barcos
        y que cuando se vayan colocando se haga delete de esos tamaños.
        PD: Hay que mirar cuales son los barcos */
        //std::vector<int> boats;

    public:
        //Constructor
        Player(std::string id, std::string password, int sd);

        //Getters
        std::string get_id(){return id_;}
        std::string get_password(){return password_;}
        std::vector<std::vector<std::string>> get_board(){return board_;}
        int get_socket(){return sd_;}

        //Setters
        void set_id(std::string id){id_ = id;}
        void set_password(std::string password){password_ = password;}

        //Methods
        bool wait();        //returns disconnetion status
        void in_game();

        void print_board();
        bool set_board();
        bool set_to_up(int size, int x, int y);
        bool set_to_right(int size, int x, int y);
        bool set_to_down(int size, int x, int y);
        bool set_to_left(int size, int x, int y);
        bool nearing_boats(int x, int y);
        void set_position(int x, int y, std::string state);

        void print_allowed_messages();
        bool check_for_right_message(char* message);

};

#endif