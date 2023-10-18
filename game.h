//
//

#ifndef GAME_H
#define GAME_H

#include "client.h"

class Game{
    private:
        Client player1_, player2_;
        bool running_;
        bool turn_player1_, turn_player2_; 
    
    public:
        //Constructor
        Game(Client player1, Client player2);

        //Getters
        Client get_player1(){return player1_;}
        Client get_player2(){return player2_;}
        bool is_running(){return running_;}
        bool is_turn_player1(){return turn_player1_;}
        bool is_turn_player2(){return turn_player2_;}

        //Setters
        void set_player1(Client client){player1_ = client;}
        void set_player2(Client client){player2_ = client;}
        void set_is_running(bool status){running_ = status;}
        void set_turn_player1(bool your_turn){turn_player1_ = your_turn;}
        void set_turn_player2(bool your_turn){turn_player2_ = your_turn;}

        //Methods
        bool start();

};

#endif