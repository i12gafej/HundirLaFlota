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
#include <mutex>
#include <map>

#define MSG_SIZE 250
#define MAX_CLIENTS 30

//
//


class Server{
    private:
        std::vector<std::tuple<std::string, std::string>> logins;
        std::vector<std::tuple<std::string, int>> validados;
        /**
         * Hay que hacer un sistema que nos permita guardar el descriptor del usuario 
         * y asignarlo para toda la ejecución del programa, con lo que ahora validados
         * std::vector<std::string> validados;
         *                 | |
         *                  V
         * std::vector<std::tuple<std::string, int>> validados;
         * con user y descriptor
         * 
        */
        std::map<int, std::string> sd_dict;
    public:

    bool start();
    bool stop();
    inline bool start_game(){return false;};//puede ser q la necesite
    void close_client(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);

    //get
    inline std::vector<std::tuple<std::string, std::string>> getLogins(){return this->logins;};
    inline std::vector<std::tuple<std::string, int>> getValid(){return this->validados;};
    inline std::string getUserBySd(int sd){return sd_dict.at(sd);};
    inline std::map<int, std::string> getDict(){return sd_dict;};
        
    
    //set
    inline void pushbackValid(std::string string, int sd){this->validados.push_back(std::tuple<std::string, int>(string,sd));};
    inline void setUserInDict(std::string user, int sd){this->sd_dict[sd] = user;};
        //sets the array form the I/O file
    void setLoginArray();
    
    //print 
        //prints the array to the I/O file
    void printLoginArray();
    //method
    void addLogin(std::string login, std::string password){this->logins.push_back(std::tuple<std::string, std::string>(login, password));};
    bool addValid(std::string login, int sd); // devuelve falso si a quien añades ya está validado
    bool checkLogin(std::string string);
    bool checkPassword(std::string l, std::string p);
    bool userInDict(std::string user, int sd);

};
void manejador (int signum);
char * tratarString(char * buffer);

#endif