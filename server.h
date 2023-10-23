#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h>
#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include "player.h"
#include <tuple>

#define MSG_SIZE 250
#define MAX_CLIENTS 30

//
//


class Server{
    private:
        std::vector<std::tuple<std::string, std::string>> logins;
        std::vector<std::string> validados;
    public:

    bool start();
    bool stop();
    inline bool start_game(){return false;};//puede ser q la necesite
    void close_client(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);

    //get
    inline std::vector<std::tuple<std::string, std::string>> getLogins(){return this->logins;};
    inline std::vector<std::string> getValid(){return this->validados;};
    //set
        //sets the array form the I/O file
    void setLoginArray(std::vector<std::tuple<std::string, std::string>> array);
    inline void pushbackValid(std::string string){this->validados.push_back(string);};
    //method
    inline void addLogin(std::string login, std::string password){this->logins.push_back(std::tuple<std::string, std::string>(login, password));};
    bool addValid(std::string login); // devuelve salfo si a quien añades ya está validado
    bool checkLogin(std::string string);
    bool checkPassword(std::string l, std::string p);

};
void manejador (int signum);

#endif