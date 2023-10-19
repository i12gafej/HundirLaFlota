//
//

#include "client.h"
#include "functionality.h"

int main(){

    int sd;
	struct sockaddr_in sockname;
	char buffer[100];
	socklen_t len_sockname;
	bool disconnect = false;
	
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}

	
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  inet_addr("127.0.0.1");

	len_sockname = sizeof(sockname);
	
	if (connect(sd, (struct sockaddr *)&sockname, len_sockname) == -1)
	{
		perror ("Error de conexión");
		exit(1);
	}

	printf("Conectado al servidor.\n");

    do{                                 //probably to be deleted
        char action[100];

        std::cout << "Introduzca una orden\n";
        fgets(action, sizeof(action), stdin);
        action[strlen(action)-1] = '\0';

        if(strcmp(buffer, "SALIR") == 0){               //no need for more actions
            disconnect = true;
        }
        else
        {
            if(send(sd, action, sizeof(action), 0) == -1) {
            	std::cout << "Error al enviar el mensaje\n";
       	    	exit(EXIT_FAILURE);
   		    }

            std::vector<std::string> placeholder = split(action, ' ');
            std::string username = placeholder[1];

            bzero(action, sizeof(action));
   		    if (recv(sd, action, sizeof(action), 0) == -1) {
        		std::cout << "Error al recibir la respuesta\n";
        		exit(EXIT_FAILURE);
    		}

            /*OJO A PARTIR DE AQUÍ, HAY QUE ESPERAR A LA RESPUESTA DEL SERVER DE JAVI*/

            fgets(action, sizeof(action), stdin);
            action[strlen(action)-1] = '\0';

            placeholder = split(action, ' ');
            std::string password = placeholder[1];

            Client client = Client(username, password, sd);
            
            while(true){
                disconnect = client.wait();
                if(disconnect){
                    break;
                }
                
            }
            


        }



    }while(!disconnect);
    



}