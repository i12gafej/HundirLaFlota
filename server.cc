//
//

#include "server.h"



bool Server::start(){
    int sd, new_sd;
	struct sockaddr_in sockname, from;
	char cbuffer[MSG_SIZE];
    std::string buffer;
	socklen_t from_len;
    fd_set readfds, auxfds;
   	int salida;
   	int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
   	 //contadores
    int i,j,k;
	int recibidos;
   	char identificador[MSG_SIZE];
    
    int on, ret;

    
    
	/* --------------------------------------------------
		Se abre el socket 
	---------------------------------------------------*/
  	sd = socket (AF_INET, SOCK_STREAM, 0);
	if (sd == -1)
	{
		perror("No se puede abrir el socket cliente\n");
    		exit (1);	
	}
    
    	// Activaremos una propiedad del socket para permitir· que otros
    	// sockets puedan reutilizar cualquier puerto al que nos enlacemos.
    	// Esto permite· en protocolos como el TCP, poder ejecutar un
    	// mismo programa varias veces seguidas y enlazarlo siempre al
   	 // mismo puerto. De lo contrario habrÌa que esperar a que el puerto
    	// quedase disponible (TIME_WAIT en el caso de TCP)
    	on=1;
    	ret = setsockopt( sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));



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
		perror("Error en la operación de listen");
		exit(1);
	}

	
    	printf("El servidor está esperando conexiones...\n");	//Inicializar los conjuntos fd_set
    	
	FD_ZERO(&readfds);
    FD_ZERO(&auxfds);
    FD_SET(sd,&readfds);
    FD_SET(0,&readfds);
    

    //Capturamos la señal SIGINT (Ctrl+c)
    signal(SIGINT,manejador);
    //SIEMPRE ESTA ESPERANDO COSAS
    while(1){
            
            //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
            
            auxfds = readfds;
            //select te da el numero de descriptores + 1 activos
            salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
            
            //si ha leido
            if(salida > 0){
                
                //recorre los descriptores
                for(i=0; i<FD_SETSIZE; i++){
                    
                    //Buscamos el socket por el que se ha establecido la comunicación
                    if(FD_ISSET(i, &auxfds)) {
                        //si el socket con el que se ha establecido la conexion es el que buscamos
                        if( i == sd){
                            //asignamos el nuevo sd del actual
                            if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1){
                                perror("Error aceptando peticiones");
                            }
                            else
                            {
                                //siempre que no se haya excedido el maximo de clientes
                                if(numClientes < MAX_CLIENTS){
                                    arrayClientes[numClientes] = new_sd;
                                    numClientes++;
                                    FD_SET(new_sd,&readfds);
                                
                                    //strcpy(buffer, "Bienvenido al chat\n");
                                    buffer = "+0k. Usuario conectado";
                                    
                                    send(new_sd,buffer.c_str(),sizeof(buffer.c_str()),0);
                                }
                                else
                                {
                                    //bzero(buffer,sizeof(buffer));
                                    buffer = "";
                                    buffer = "Demasiados clientes conectados\n";
                                    send(new_sd,buffer.c_str(),sizeof(buffer.c_str()),0);
                                    close(new_sd);
                                }
                                
                            }
                            
                            
                        }
                        else if (i == 0){
                            //Se ha introducido información de teclado
                            bzero(cbuffer, sizeof(cbuffer));
                            buffer = "";
                            fgets(cbuffer, sizeof(cbuffer),stdin);
                            buffer.assign(cbuffer);
                            //Mensajes que se quieran mandar a los clientes (implementar)
                            
                        } 
                        else{
                            bzero(cbuffer,sizeof(cbuffer));
                            buffer = "";
                            recibidos = recv(i,cbuffer,sizeof(cbuffer),0);
                            buffer.assign(cbuffer);
                            std::string aux;
                            if(recibidos > 0){
                                
                                if(buffer == "SALIR\n"){
                                    
                                    close_client(i,&readfds,&numClientes,arrayClientes);
                                    
                                }
                                else if(buffer.substr(0, 8) == "USUARIO "){
                                    aux = buffer.substr(8, buffer.size());
                                    //if (indice != std::string::npos) {


                                }
                                else{
                                    
                                    sprintf(identificador,"<%d>: %s",i,buffer);
                                    //bzero(buffer,sizeof(buffer));
                                    buffer = "";
                                    //strcpy(buffer,identificador);
                                    buffer.assign(identificador);

                                    std::cout << buffer <<std::endl;

                                    for(j=0; j<numClientes; j++)
                                        if(arrayClientes[j] != i)
                                            send(arrayClientes[j],buffer.c_str(),sizeof(buffer.c_str()),0);

                                    
                                }
                                                                
                                
                            }
                            //Si el cliente introdujo ctrl+c
                            if(recibidos== 0)
                            {
                                printf("El socket %d, ha introducido ctrl+c\n", i);
                                //Eliminar ese socket
                                close_client(i,&readfds,&numClientes,arrayClientes);
                            }
                        }
                    }
                }
            }
		}

	close(sd);
	return 0;
}

bool Server::close(){

}
void manejador (int signum){
    printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}
void Server::close_client(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]){
  
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++)
        if (arrayClientes[j] == socket)
            break;
    for (; j < (*numClientes) - 1; j++)
        (arrayClientes[j] = arrayClientes[j+1]);
    
    (*numClientes)--;
    
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente <%d>",socket);
    
    for(j=0; j<(*numClientes); j++)
        if(arrayClientes[j] != socket)
            send(arrayClientes[j],buffer,sizeof(buffer),0);


}
bool Server::checkLogin(std::string string){
    size_t ;
}