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

#define MSG_SIZE 250
#define MAX_CLIENTS 30

//
//


class Server{
    public:

    bool start();
    bool close();
    void close_client(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]);

};
void manejador (int signum);

#endif