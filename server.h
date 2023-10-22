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

#define MSG_SIZE 250
#define MAX_CLIENTS 30

//
//


class Server{
    private:
        std::vector<std::string> logins;
    public:

    bool start();
    bool close();
    void close_client(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);

    //get
    inline std::vector<std::string> getLogins(){return this->logins;};
    //set
        //sets the array form the I/O file
    void setLoginArray(std::vector<std::string> array);
    //method
    inline void addLogin(std::string string){this->logins.push_back(string);};
    bool checkLogin(std::string string);

};
void manejador (int signum);

#endif