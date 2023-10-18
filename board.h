//
//

#ifndef BOARD_H
#define BOARD_H

#include <vector>

class Board{
    private:
        //Size of the matrix is reserved in the constructor
        std::vector<std::vector<std::string>> board_;    //coordinates from one of the axis will be required to be translated
        int n_boats_;
        bool ready_;

        /*We need to somehow store the particular TYPES of boats that remain to be put in the board*/
    public:
        //Constructor
        Board();

        //Getters
        int get_nboats(){return n_boats_;}
        bool is_ready(){return ready_;}

        //Setters
        void set_nboats(int n_boats){n_boats_ = n_boats;}
        void set_is_ready(bool ready){ready_ = ready;}
        

        //Methods
        void initialise_board();
        void print_board();

};

#endif