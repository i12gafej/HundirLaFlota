#include "server.h"

void conexionCliente(int new_sd, char* buffer){         //cuidado con el char*
    int salir = 0;
    do{
		// Recibir datos del cliente
		bzero(buffer, sizeof(buffer));
		if(recv(new_sd, buffer, sizeof(buffer), 0) == -1)
		{
			printf("[ERROR]: no se puede leer del cliente\n%d: %s\n", errno, strerror(errno));
			exit(EXIT_FAILURE);
        }

        //HUELE A SWITCH A ESTA ALTURA DEPENDIENDO DEL MENSAJE (y va a depender de un strcmp)
		if(strcmp(buffer, "FIN") == 0){
			salir = 1;
        }
		
		printf("El mensaje recibido fue: %s\n", buffer );

		// Enviar respuesta al cliente
		bzero(buffer, sizeof(buffer));
		strcpy(buffer, "Ok. Mensaje recibido");
		if(send(new_sd, buffer, sizeof(buffer), 0) == -1){
			perror("Error en la operación de send");
			exit(1);
		}
					
			
	}while(!salir);
}