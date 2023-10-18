//
//

#ifndef CLIENT_H
#define CLIENT_H

#include <string>
#include <vector>

class Client{
    private:
        int id_;
        std::string password_;
        std::vector<std::vector<std::string>> board_;
        bool waiting;

        /*Había pensado en algo así como tener un vector con los el tamaño de los barcos
        y que cuando se vayan colocando se haga delete de esos tamaños.
        PD: Hay que mirar cuales son los barcos */
        //std::vector<int> boats;

    public:
        //Constructor
        Client(int id, std::string password);

        //Getters
        int get_id(){return id_;}
        std::string get_password(){return password_;}
        std::vector<std::vector<std::string>> get_board(){return board_;}

        //Setters
        void set_id(int id){id_ = id;}
        void set_password(std::string password){password_ = password;}

        //Methods
        bool connect();
        void print_board();
        bool set_board();
        bool set_to_up(int size, int x, int y);
        bool set_to_right(int size, int x, int y);
        bool set_to_down(int size, int x, int y);
        bool set_to_left(int size, int x, int y);

};

#endif