#include "server.h"

int main(){

    //Atributos del server
    int sd, new_sd;
	struct sockaddr_in sockname, from;
	char buffer[100];
	socklen_t from_len;
	int salir = 0;
    int nConexiones;       //controla el maximo de conexiones

    //Apertura de socket
    sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		printf("[SERVER]: error al abrir el socket del cliente\n%d: %s\n", errno, strerror(errno));
        exit(EXIT_FAILURE);
	}

    //Atributos del socket
	sockname.sin_family = AF_INET;
	sockname.sin_port = htons(2000);
	sockname.sin_addr.s_addr =  INADDR_ANY;

    //Vincula el socket y le asocia un puerto
	if (bind (sd, (struct sockaddr *) &sockname, sizeof (sockname)) == -1)
	{
		printf("[SERVER]: error en el enlace\n%d: %s\n", errno, strerror(errno));
		exit(EXIT_FAILURE);
	}

    from_len = sizeof (from);

    //Pone el socket en modo escucha
	if(listen(sd,1) == -1){
		printf("[SERVER]: error en el listen\n%d: %s\n", errno, strerror(errno));
		exit(1);	
    }

    //el servidor se pone en modo espera
    while(1){
		
            if(nConexiones <= 30){
                // Aceptar una conexión entrante
                if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
				printf("[SERVER]: error al recibir la petición\n%d: %s\n", errno, strerror(errno));
				exit(1);
			}
                nConexiones++;
                printf("Cliente conectado desde %s:%d\n", inet_ntoa(from.sin_addr), ntohs(from.sin_port));

                pid_t pid = fork();     //seguramente, lo que haga el fork serán partidas, no clientes (?)

                switch(pid){
                    case -1:
                        printf("[SERVER]: error al gestionar el cliente\n%d: %s\n",
                            errno, strerror(errno));
                        exit(EXIT_FAILURE);
                    break;
                    case 0:
                        conexionCliente(new_sd, buffer);
                    break;
                    default:
                    break;
                }

                int status;
                if(wait(&status) < 0){
                    printf("[SERVER]: error al terminar el cliente\n");
                    exit(EXIT_FAILURE);
                }else{
                    if(WIFEXITED(status)){
                        printf("[SERVER]: cliente desconectado con estado: %d\n", WEXITSTATUS(status));
                    }else{
                        printf("[SERVER]: cliente desconectado de manera anormal\n");
                    }
                }
			    nConexiones--;

			    close(new_sd);
            
            }else{

            }
			
		}

		close(sd);

    exit(EXIT_SUCCESS);
}

/*NOTAS:
    Cuando se conecta un cliente, se debe acudir a un fichero de texto plano en el que están
las cuentas de los usuarios que se han conectado en durante la sesión. Esto debería implementarse
en dos funciones: 1. revisaUsuario y 2.registraUsuario.*/