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
#include <errno.h>
#include <iostream>

class Player{
    private:
        std::string id_;
        std::string password_;
        std::vector<std::vector<char>> board_;
        std::vector<std::vector<char>> contrary_board_;
        int sd_;
        bool waiting;
        int shots_;
    public:
        //Constructor
        Player(std::string id, std::string password, int sd);
        Player(){}

        //Getters
        std::string get_id(){return id_;}
        std::string get_password(){return password_;}
        std::vector<std::vector<char>> get_board(){return board_;}
        int get_socket(){return sd_;}
        int get_shots(){return shots_;}

        //Setters
        void set_id(std::string id){id_ = id;}
        void set_password(std::string password){password_ = password;}
        void shoot(){shots_++;}

        //Methods
        void in_game();

        void print_board();
        void print_contrary_board();
        bool set_board();
        bool set_to_up(int size, int x, int y);
        bool set_to_right(int size, int x, int y);
        bool set_to_down(int size, int x, int y);
        bool set_to_left(int size, int x, int y);
        bool nearing_boats(int x, int y);
        bool out_of_limits(int x, int j);
        void set_position(int x, int y, char state);

};

#endif