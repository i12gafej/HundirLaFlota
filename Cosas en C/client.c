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
#include "client.h"

int main(){
    /*Crea el client*/

    int sd;
	struct sockaddr_in sockname;
	char buffer[100];
	socklen_t len_sockname;
	int salir = 0;

    sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		printf("[CLIENT]: error al abrir el socket\n%d: %s\n", errno, strerror(errno));
        exit (EXIT_FAILURE);	
	}

    sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  inet_addr("127.0.0.1");     //seguramente haya que ver cómo conectarse a una sola conexión

    len_sockname = sizeof(sockname);
	
	if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1)
	{
		printf("[CLIENT]: error al conectar\n%d: %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("Conectado al servidor\n");

    //No hace falta tener control de procesos porque este es el mismo proceso (?)
    do
	{
		salir = 0;
		puts("Teclee el mensaje a transmitir");
		fgets(buffer, sizeof(buffer),stdin);
		buffer[strlen(buffer)-1] = '\0';
			
		if(strcmp(buffer, "FIN") == 0)
			salir = 1;

		// Enviar un mensaje al servidor
		if (send(sd, buffer, sizeof(buffer), 0) == -1) {
        		perror("Error al enviar el mensaje");
       			 exit(1);
   		 }

		// Recibir respuesta del servidor
   		bzero(buffer, sizeof(buffer));
   		if (recv(sd, buffer, sizeof(buffer), 0) == -1) {
        		perror("Error al recibir la respuesta");
        		exit(1);
    		}

    		printf("Respuesta del servidor: %s\n", buffer);

				
	}while(!salir);
	
	// Cerrar el socket del cliente	
	close(sd);

    exit(EXIT_SUCCESS);
}