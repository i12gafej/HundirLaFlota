//
//

#include "client.h"

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

    do{
        char action[100];

        std::cout << "Introduzca una orden\n";
        fgets(action, sizeof(action), stdin);
        action[strlen(action-1)] = action;

        if(strcmp(buffer, "SALIR") == 0){               //no need for more actions
            disconnect = true;
        }
        else
        {
            if(send(sd, action, sizeof(action), 0) == -1) {
            	std::cout << "Error al enviar el mensaje\n";
       	    	exit(EXIT_FAILURE);
   		    }

            /*TODO*/
            std::string placeholder1(action);
            std::vector<std::string> parts1;
            std::string part1;

            while (std::getline(placeholder1, part1, " ")) {
                parts1.push_back(part1);
            }

            std::string username = part[1];


            bzero(action, sizeof(action));
   		    if (recv(sd, action, sizeof(action), 0) == -1) {
        		std::cout << "Error al recibir la respuesta\n";
        		exit(EXIT_FAILURE);
    		}

            /*OJO A PARTIR DE AQUÍ, HAY QUE ESPERAR A LA RESPUESTA DEL SERVER DE JAVI*/
            fgets(action, sizeof(action), stdin);
            action[strlen(action-1)] = action;

            std::string placeholder2(action);
            std::vector<std::string> parts2;
            std::string part2;

            while (std::getline(placeholder2, part2, " ")) {
                parts2.push_back(part2);
            }

            std::string password = part[1];

            Client(username, password);

            /*Cositas de client al fin coño*/

        }



    }while(!disconnect);
    



}