//
//

#include "server.h"
#include <errno.h>


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
    
    bool flag_user = false, flag_pass = false, flag_singup = false, flag_init = false; 
    //Capturamos la señal SIGINT (Ctrl+c)
    //signal(SIGINT,manejador);
    //SIEMPRE ESTA ESPERANDO COSAS
    while(1){
        //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
        
        auxfds = readfds;
        //select te da el numero de descriptores + 1 activos
        salida = select(FD_SETSIZE,&auxfds,NULL,NULL,NULL);
        
        //si ha leido
        if(salida > 0){
            
            //recorre los descriptores
            for(i=0; i<FD_SETSIZE; i++)
            {    
                //Buscamos el socket por el que se ha establecido la comunicación
                if(FD_ISSET(i, &auxfds))
                {
                    //si el socket con el que se ha establecido la conexion es el que buscamos
                    if( i == sd)
                    {
                        //asignamos el nuevo sd del actual
                        if((new_sd = accept(sd, (struct sockaddr *)&from, &from_len)) == -1)
                        {
                            printf("Error aceptando peticiones\n%d: %s", errno, strerror(errno));
                        }
                        else
                        {
                            //siempre que no se haya excedido el maximo de clientes
                            if(numClientes < MAX_CLIENTS){
                                arrayClientes[numClientes] = new_sd;
                                numClientes++;
                                FD_SET(new_sd,&readfds);
                            
                                //strcpy(buffer, "Bienvenido al chat\n");
                                buffer = "+Ok. Usuario conectado\n";
                                for(j=0; j<(numClientes-1);j++){
                                    send(new_sd,buffer.c_str(),sizeof(buffer.c_str()),0);
                                }
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
                    else if (i == 0)
                    {
                        //Se ha introducido información de teclado
                        bzero(cbuffer, sizeof(cbuffer));
                        buffer = "";
                        fgets(cbuffer, sizeof(cbuffer),stdin);
                        buffer.assign(cbuffer);
                        //Mensajes que se quieran mandar a los clientes (implementar)
                        
                    } 
                    else
                    {
                        bzero(cbuffer,sizeof(cbuffer));
                        buffer = "";
                        recibidos = recv(i,cbuffer,sizeof(cbuffer),0);
                        buffer.assign(cbuffer);
                        std::string aux, login = "nan", password;

                        if(recibidos > 0)
                        {
                            std::istringstream stream(buffer);
                            stream >> aux;
                            if(aux == "SALIR")
                            {   
                                printf("El socket %d ha abandonado el servidor\n", i);
                                close_client(i,&readfds,&numClientes,arrayClientes);   
                            }
                            else if(aux == "USUARIO")
                            {
                                stream >> login;
                                flag_user = checkLogin(aux);

                                if(flag_user)
                                {
                                    //usuario registrado
                                    buffer = "+Ok. Usuario correcto\n   Escriba la contraseña: \n";
                                    send(i, buffer.c_str(), sizeof(buffer.c_str()), 0);
                                }
                                else
                                {
                                    buffer = "-Err. Usuario incorrecto\n";
                                    send(i, buffer.c_str(), sizeof(buffer.c_str()), 0);
                                }
                            }
                            else if(aux == "PASSWORD")
                            {
                                stream >> password;
                                if(login == "nan")
                                {
                                    buffer = "Hay que añadir el usuario\n";
                                    send(i, buffer.c_str(), sizeof(buffer.c_str()), 0);
                                }
                                else
                                {
                                    flag_pass = checkPassword(login, password);
                                
                                    if(flag_pass)
                                    {
                                        buffer = "+Ok. Usuario validado\n";
                                        pushbackValid(login);
                                        send(i, buffer.c_str(), sizeof(buffer.c_str()), 0);
                                    }
                                    else
                                    {
                                        buffer = "-Err. Error en la validación\n";
                                        send(i, buffer.c_str(), sizeof(buffer.c_str()), 0);
                                    }
                                }
                                
                                
                            }
                            else if(aux == "REGISTRO")
                            {
                                stream >> aux;
                                if(aux != "-u")
                                {
                                    buffer = "Error de formato\nEnviar REGISTRO -u usuario -p password\n";
                                }
                                else
                                {
                                    stream >> login;
                                    stream >> aux;
                                    if(aux != "-p"){
                                        buffer = "Error de formato\nEnviar REGISTRO -u usuario -p password\n";
                                    }
                                    else
                                    {
                                        stream >> password;

                                        if(checkLogin(login))
                                        {
                                            buffer = "Usuario ya registrado\n";
                                        }
                                        else
                                        {
                                            addLogin(login, password);
                                            pushbackValid(login);
                                            buffer = "Usuario validado\n";
                                            flag_singup = true;
                                        }
                                        
                                        //flag_pass = true;
                                        //flag_user = true;
                                    }
                                }
                                send(i, buffer.c_str(), sizeof(buffer.c_str()), 0);
                            }
                            //else if()
                            //queda mas


                            
                        }   
                            //Si el cliente introdujo ctrl+c
                        if(recibidos == 0)
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

bool Server::stop(){
    return true;
}
void manejador (int signum){
    /*printf("\nSe ha recibido la señal sigint\n");
    signal(SIGINT,manejador);*/
    
    //Implementar lo que se desee realizar cuando ocurra la excepción de ctrl+c en el servidor
}

void Server::close_client(int socket, fd_set * readfds, int * numClientes, int arrayClientes[]){
  
    char buffer[250];
    int j;
    
    close(socket);
    FD_CLR(socket,readfds);
    
    //Re-estructurar el array de clientes
    for (j = 0; j < (*numClientes) - 1; j++){
        if (arrayClientes[j] == socket)
            break;
    }
        
    for (; j < (*numClientes) - 1; j++){
         (arrayClientes[j] = arrayClientes[j+1]);
    }
       
    
    (*numClientes)--;
    
    bzero(buffer,sizeof(buffer));
    sprintf(buffer,"Desconexión del cliente <%d>",socket);
    
    for(j=0; j<(*numClientes); j++){
        if(arrayClientes[j] != socket)
            send(arrayClientes[j],buffer,sizeof(buffer),0);
    }
        


}
bool Server::checkLogin(std::string string){
    std::vector<std::tuple<std::string, std::string>> loginArray = getLogins();
    for(int i = 0; i < loginArray.size(); i++){
        if(string == std::get<0>(loginArray[i])){
            return true;
        }
    }
    return false;
}
bool Server::checkPassword(std::string l, std::string p){
    std::vector<std::tuple<std::string, std::string>> loginArray = getLogins();
    for(int i = 0; i < loginArray.size(); i++){
        if(l == std::get<0>(loginArray[i]) && p == std::get<1>(loginArray[i])){
            return true;
        }
    }
    return false;
}
void Server::setLoginArray(std::vector<std::tuple<std::string, std::string>> array){
    std::ifstream f;
    f.open("login.txt", std::ios::in);
    if(f.is_open()){
        std::string linea;
        while(std::getline(f, linea)){
            std::istringstream stream(linea);
            std::string usuario, contrasena;
            
            stream >> usuario >> contrasena;
            addLogin(usuario, contrasena);
        }
        f.close();
    }
    else{
        std::cerr << "No se pudo abrir el archivo \"login.txt\"." << std::endl;
    }
}
bool Server::addValid(std::string login){
    auto old_valid = getValid();
    for(int i = 0; i < old_valid.size(); i++){
        if(login == old_valid[i]){
            return false;
        }
    }
    pushbackValid(login);
    return true;

}