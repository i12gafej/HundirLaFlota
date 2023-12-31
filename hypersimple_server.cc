//
//

#include "server.h"
#include <errno.h>


bool Server::start(){
    int sd, new_sd;
	struct sockaddr_in sockname, from;
	char buffer[MSG_SIZE];
    std::string sbuffer;
	socklen_t from_len;
    fd_set readfds, auxfds;
   	int salida;
   	int arrayClientes[MAX_CLIENTS];
    int numClientes = 0;
   	 //contadores
    int i,j,k;
	ssize_t recibidos;
   	char identificador[MSG_SIZE];
    
    int on, ret;

    //setLoginArray();

    
    
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
    //signal(SIGINT,manejador);                 //temporally deprecated

    //SIEMPRE ESTA ESPERANDO COSAS
    while(1){
        //Esperamos recibir mensajes de los clientes (nuevas conexiones o mensajes de los clientes ya conectados)
        
        auxfds = readfds;
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
                            if(numClientes < MAX_CLIENTS)
                            {
                                arrayClientes[numClientes] = new_sd;
                                numClientes++;
                                FD_SET(new_sd,&readfds);

                                bzero(buffer,sizeof(buffer));
                                strcpy(buffer, "+Ok. Usuario conectado\n");
                                send(new_sd,buffer,sizeof(buffer),0);

                                //buffer = "+Ok. Usuario conectado\n";
                                for(j=0; j<(numClientes-1);j++){
                                    bzero(buffer,sizeof(buffer));
                                    sprintf(buffer, "Nuevo Cliente conectado en <%d>",new_sd);
                                    send(arrayClientes[j],buffer,sizeof(buffer),0);
                                }
                            }
                            else
                            {
                                bzero(buffer,sizeof(buffer));
                                //buffer = "";
                                sprintf(buffer, "Demasiados clientes conectados\n");
                                //buffer = "Demasiados clientes conectados\n";
                                send(new_sd,buffer,sizeof(buffer),0);
                                close(new_sd);
                            }
                            
                        }
                        
                    }
                    else //if (i == 0)
                    {

                        //bzero(buffer, sizeof(buffer));
                        //fgets(buffer, sizeof(buffer),stdin);
                        
                        bzero(buffer,sizeof(buffer));
                        char *usuario, *contra;
                        sbuffer = "";
                        recibidos = recv(i,buffer,sizeof(buffer),0);
                        sbuffer.assign(buffer);
                        
                        //std::istringstream stream(sbuffer);

                        if(recibidos > 0)
                        {
                            Player player1("1", "foo", i);
                            Player player2("2", "foo", i);
                            Game game(player1, player2);

                            bool end;
                            end = game.start();

                            bzero(buffer, sizeof(buffer));
                            if(end){
                                sprintf(buffer, "Ha ganado %s con %d disparos\n", player1.get_id().c_str(),
                                    player1.get_shots());
                                send(player1.get_socket(), buffer, sizeof(buffer), 0);
                            }else{
                                sprintf(buffer, "Ha ganado %s con %d disparos\n", player2.get_id().c_str(),
                                    player2.get_shots());   
                                send(player1.get_socket(), buffer, sizeof(buffer), 0);
                            }
                            
                        }
                        //Si el cliente introdujo ctrl+c
                        if(recibidos == 0)
                        {
                            printf("El socket %d, ha introducido ctrl+c\n", i);
                            //Eliminar ese socket
                            //close_client(i,&readfds,&numClientes,arrayClientes);
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
/*void manejador (int signum){
    
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
    auto loginArray = getLogins();
    for(auto it = loginArray.begin(); it != loginArray.end(); *it++){
        if(string == std::get<0>(*it)){
            
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
void Server::setLoginArray(){
    std::ifstream f;
    f.open("login.txt", std::ios::in);
    if(f.is_open()){
        std::string linea;
        while(std::getline(f, linea)){
            std::istringstream stream(linea);
            std::string usuario, contrasena;
            
            stream >> usuario >> contrasena;
            addLogin(usuario, contrasena);
            usuario = "";
            contrasena = "";
        }
        f.close();
    }
    else{
        std::cerr << "No se pudo abrir el archivo \"login.txt\"." << std::endl;
    }
}
bool Server::addValid(std::string login, int sd){
    auto old_valid = getValid();
    for(auto it = old_valid.begin(); it != old_valid.end(); it++){
        if(login == std::get<0>(*it)){
            return false;
        }
    }
    pushbackValid(login, sd);
    return true;

}
void Server::printLoginArray(){
    auto v = getLogins();
    std::ofstream f;
    f.open("login.txt", std::ios::out);
    if(f.is_open()){
        std::string linea;
        for(int i = 0; i < v.size(); i++){            
            f << std::get<0>(v[i]) << " " << std::get<1>(v[i]) << std::endl;
        }
        f.close();
    }
    else{
        std::cerr << "No se pudo abrir el archivo \"login.txt\"." << std::endl;
    }
}
bool Server::userInDict(std::string user, int sd){
    auto mapa = getDict();
    if(mapa.find(sd) != mapa.end()){
        if(mapa.at(sd) == user){
            return true;
        }
    }
    return false;
}
bool Server::isValidBySd(std::string user, int sd){
    auto v = getValid();
    for(auto it = v.begin(); it != v.end(); it++){
        if(user == std::get<0>(*it) && sd == std::get<1>(*it)){
            return true;
        }
    }
    return false;
}
std::string Server::getPassByUser(std::string user){
    auto v = getLogins();
    for(auto it = v.begin(); it != v.end(); it++){
        if(user == std::get<0>(*it)){
            return std::get<1>(*it);
        }
    }
    return nullptr;
}
bool Server::sdIsInGame(int sd){
    auto v = getGames();
    for(auto it = v.begin(); it != v.end(); it++){
        if(it->get_player1().get_socket() == sd || 
        it->get_player2().get_socket() == sd){
            return true;
        }
    }
    
    return false;
}
char * tratarString(char * buffer){
    char *pos = strchr(buffer, ' ');
    size_t length = strlen(pos + 1);
    char *sub = (char *)malloc(length + 1);
    strncpy(sub, pos + 1, length);
    sub[length] = '\0';
    return sub;
}
*/