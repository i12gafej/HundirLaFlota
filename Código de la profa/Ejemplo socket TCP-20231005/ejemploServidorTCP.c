#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>


/*
 * El servidor lee y muestra por pantalla la cadena que le manda el cliente hasta que recibe FIN.
 */


int main ( )
{
  
	/*---------------------------------------------------- 
		Descriptor del socket y buffer de datos                
	-----------------------------------------------------*/
	int sd, new_sd;
	struct sockaddr_in sockname, from;
	char buffer[100];
	socklen_t from_len;
	int salir = 0;


	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}


	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  INADDR_ANY;

	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
		perror("Error en la operación bind");
		exit(1);
	}
	

   	/*---------------------------------------------------------------------
		Del las peticiones que vamos a aceptar sólo necesitamos el 
		tamaño de su estructura, el resto de información (familia, puerto, 
		ip), nos la proporcionará el método que recibe las peticiones.
   	----------------------------------------------------------------------*/
		from_len = sizeof (from);


		if(listen(sd,1) == -1){
			printf("Error en la operación de listen");
			exit(1);	
		}
   	
	/*-----------------------------------------------------------------------
		El servidor acepta una petición
	------------------------------------------------------------------------ */
		while(1){
		
			// Aceptar una conexión entrante
			if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
				perror("Error aceptando peticiones");
				exit(1);
			}

			printf("Cliente conectado desde %s:%d\n", inet_ntoa(from.sin_addr), ntohs(from.sin_port));


			do
			{
				salir = 0;
				// Recibir datos del cliente
				bzero(buffer, sizeof(buffer));
				if(recv(new_sd, buffer, sizeof(buffer), 0) == -1)
				{
					perror("Error en la operación de recv");
					exit(1);
				}

				if(strcmp(buffer, "FIN") == 0)
					salir = 1;	
				
				printf("el mensaje recibido fue: %s\n", buffer );


				// Enviar respuesta al cliente
				bzero(buffer, sizeof(buffer));
				strcpy(buffer, "Ok. Mensaje recibido");
				if(send(new_sd, buffer, sizeof(buffer), 0) == -1){
					perror("Error en la operación de send");
					exit(1);
				}
					
			
			}while(!salir);
			
			close(new_sd);
		}

		close(sd);
		return 0;
	
}










