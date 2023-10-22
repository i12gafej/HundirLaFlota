//
//

#ifndef GAME_H
#define GAME_H

#include "player.h"

class Game{
    private:
        Player player1_, player2_;
        bool running_;
        bool turn_player1_;     //if its not player1 turn, its player2 
    
    public:
        //Constructor
        Game(Player player1, Player player2);

        //Getters
        Player get_player1(){return player1_;}
        Player get_player2(){return player2_;}
        bool is_running(){return running_;}
        bool is_turn_player1(){return turn_player1_;}
        //bool is_turn_player2(){return turn_player2_;}

        //Setters
        void set_player1(Player player){player1_ = player;}
        void set_player2(Player player){player2_ = player;}
        void set_is_running(bool status){running_ = status;}
        void set_turn_player1(bool your_turn){turn_player1_ = your_turn;}
        //void set_turn_player2(bool your_turn){turn_player2_ = your_turn;}

        //Methods
        bool start();
        char* attack(bool player_turn, int coord_x, int coord_y);     //returns message of state of the attack

};

#endif