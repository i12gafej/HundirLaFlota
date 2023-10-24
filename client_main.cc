//
//

#include "player.h"
#include "functionality.h"
#include <errno.h>

int main(){

    int sd;
	struct sockaddr_in sockname;
	char buffer[200];
	socklen_t len_sockname;
	bool disconnect = false;

    fd_set readfds, auxfds;
    int placeholder;
	
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		printf("Error al abrir el socket del cliente\n%d: %s\n", errno, strerror(errno));
    	exit (EXIT_FAILURE);	
	}

	
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  inet_addr("127.0.0.1");

	len_sockname = sizeof(sockname);
	
	if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1)
	{
		printf("Error de conexion\n%d: %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

	printf("Conectado al servidor: %d.\n", sd);


    //Initialization of socket structs
    FD_ZERO(&auxfds);
    FD_ZERO(&readfds);
    FD_SET(0, &readfds);
    FD_SET(sd, &readfds);

    do{

        auxfds = readfds;
        placeholder = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);    //for socket status

        if(placeholder <= 0){
            printf("ERROR en el select\n %d: %s\n", errno, strerror(errno));
            exit(EXIT_FAILURE);
        }

        if(FD_ISSET(sd, &auxfds)){                                  //server response
            bzero(buffer, sizeof(buffer));
            if (recv(sd, buffer, sizeof(buffer), 0) == -1) {
        		printf("ERROR al recibir respuesta\n%d: %s\n", errno, strerror(errno));
        		exit(EXIT_FAILURE);
    		}
            //std::cout << "HOLAA" <<std::endl;
            printf("%s", buffer);

            if(strcmp(buffer, "-Err. Demasiados clientes conectados") == 0){
                disconnect = true;
            }
            else if(strcmp(buffer, "+Ok. Desconexion servidor") == 0){
                disconnect = true;
            }
            
        }else{

            if(FD_ISSET(0, &auxfds)){           //it's client turn to talk to the server
                
                bzero(buffer, sizeof(buffer));
                fgets(buffer, sizeof(buffer), stdin);
                //buffer[strlen(buffer)-1] = '\0';

                if(strcmp(buffer, "SALIR\n") == 0){
                    disconnect = true;
                }
                else
                {
                    if(send(sd, buffer, sizeof(buffer), 0) == -1) {
            	    printf("Error al enviar el mensaje\n%d: %s\n", errno, strerror(errno));
       	    	    exit(EXIT_FAILURE);
   		            }
                }
            }
        }
            
    }while(!disconnect);
    
}
