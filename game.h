//
//

#ifndef GAME_H
#define GAME_H

#include "player.h"

class Game{
    private:
        Player player1_, player2_;
        bool end_;
        bool turn_player1_;     //if its not player1 turn, its player2 
    
    public:
        //Constructor
        Game(Player player1, Player player2);

        //Getters
        Player get_player1(){return player1_;}
        Player get_player2(){return player2_;}
        bool has_ended(){return end_;}
        bool is_turn_player1(){return turn_player1_;}
        //bool is_turn_player2(){return turn_player2_;}

        //Setters
        void set_player1(Player player){player1_ = player;}
        void set_player2(Player player){player2_ = player;}
        void set_has_ended(bool status){end_ = status;}
        void set_turn_player1(bool your_turn){turn_player1_ = your_turn;}
        //void set_turn_player2(bool your_turn){turn_player2_ = your_turn;}

        //Methods
        bool start();
        int coordinate_conversion_engine(char* s);
        std::string attack(bool player_turn, char* x, char* y);     //returns message of state of the attack
        bool ckeck_game_ended(bool turn);
        void commute_player_turn();
        bool right_coordinates(char* x, char* y);

};

#endif

/*NOTAS:
    Para esperar la recepción del server, se puede hacer una función tal que esté en un bucle while
infinito hasta que se pueda leer del server.
    Una aproximación puede ser asignar una variable siempre que se llame a la función al valor
que va a permitir el bucle y que esté checkando siempre si se ha cambiado. La función de escucha
o lo que sea está dentro del bucle y en cuanto se cambie se romperá el bucle y se obtendrán las
coordenadas del disparo.*/