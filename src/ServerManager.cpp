#include "ServerManager.hpp"

//static bool running = true;
sem_t semaphore; 

/**
 * @brief The constructor for the ServerManager class
 */
ServerManager::ServerManager(ConfigParser::Server server_conf, int socket) {
    this->server_confs.push_back(server_conf);
    this->server_fd = socket;

    int ret = pthread_create(&monitor_thread, NULL, &ServerManager::monitor_exit_command_static, this); 
    if (ret != 0) {
        std::cerr << "Error al crear el hilo: " << strerror(ret) << std::endl;
        // Manejar el error (por ejemplo, salir del programa)
        exit(1); 
    }
}

ServerManager::~ServerManager() {}

/**
 * @brief The handlePost function handles HTTP POST requests containing file data sent by the client.
 * 
 * @param request The request that has been sent to the server.
 * @param server_root the path were the servers static files are located.
 * 
 * @details line 1 of the function to line 4 -> find the end of the header, if the delimiter is not found an error 400 is send.
 * line 6 of the function to line 10 -> extract the headers of the request and verify Content-Lenght, if is not present an error 411(Lenght Required) is send.
 * line 12 of the function to line 15 -> verify the content is multipart/form-data, if is not specified an error 415(Unsupported Media Type) is send.
 * line 17 of the function to line 24 -> obtain the multipart delimiter, on failure an error 400 is send.
 * line 26 of the function to line 38 -> extract the request body, on failure an error 400 is send.
 * line 40 of the function to line 46 -> extract the name of the file, on failure an error 400 is send.
 * line 48 of the function to line 53 -> extract the file data, on faiure an error 400 is send.
 * line 55 of the function to line 62 -> save the file in the server.
 * 
 * 
 * @returns on failure a Error Message with its coresponding id, on success a Success Message with its coresponding id.
 */

void* ServerManager::monitor_exit_command_static(void* arg) {
    ServerManager* this_ptr = static_cast<ServerManager*>(arg);
    this_ptr->monitor_exit_command(); 
	return NULL;
}

void ServerManager::monitor_exit_command() {
    std::string input;
    while (running) {
        std::getline(std::cin, input);
        if (input == "exit") {
            std::cout << "Exit command received. Shutting down server...\n";
			running = false;
			break;
        }
    }
    exit(1); 
}

std::string ServerManager::handlePostUpload(std::string request, std::string server_root) {
    std::size_t header_end = request.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        return HTTP400;
    }

    std::string headers = request.substr(0, header_end);
    std::size_t content_length_pos = headers.find("Content-Length: ");
    if (content_length_pos == std::string::npos) {
        return HTTP411;
    }

    std::size_t content_type_pos = headers.find("Content-Type: multipart/form-data;");
    if (content_type_pos == std::string::npos) {
        return HTTP415;
    }

    std::string boundary_prefix = "boundary=";
    std::size_t boundary_pos = headers.find(boundary_prefix, content_type_pos);
    if (boundary_pos == std::string::npos) {
        return HTTP400;
    }
    std::string boundary = "--" + headers.substr(boundary_pos + boundary_prefix.size());
    boundary = boundary.substr(0, boundary.find("\r\n"));

    std::string body = request.substr(header_end + 4);
    std::size_t file_start = body.find(boundary);
    if (file_start == std::string::npos) {
        return HTTP400;
    }
    file_start += boundary.size() + 2;

    std::size_t file_end = body.find(boundary, file_start);
    if (file_end == std::string::npos) {
        return HTTP400;
    }
    std::string file_content = body.substr(file_start, file_end - file_start);

    std::size_t filename_pos = file_content.find("filename=\"");
    if (filename_pos == std::string::npos) {
        return HTTP400;
    }
    std::string filename = file_content.substr(filename_pos + 10);
    filename = filename.substr(0, filename.find("\""));

    std::size_t data_start = file_content.find("\r\n\r\n");
    if (data_start == std::string::npos) {
        return HTTP400;
    }
    data_start += 4;
    std::string file_data = file_content.substr(data_start);

    std::string file_path = server_root + "/" + filename;
    int fd = open(file_path.c_str(), O_CREAT | O_WRONLY, 0666);
    if (fd < 0) {
        return HTTP500;
    }
    write(fd, file_data.c_str(), file_data.size());
    close(fd);

    return "HTTP/1.1 201 Created\r\nContent-Type: text/html\r\n\r\n<h1>File Uploaded Successfully</h1>";
}



std::string ServerManager::handlePost(std::string request, std::string request_path, std::string server_root) {

    // Busco el fin de la cabecera HTTP si no lo encuentro envio un error 400
    std::size_t header_end = request.find("\r\n\r\n");
    if (header_end == std::string::npos) {
        return HTTP400;
    }

    // Busco en la cabecera donde está el tamaño del contenido que se envía, si no está envío un error 411
    std::string headers = request.substr(0, header_end);
    std::size_t content_length_pos = headers.find("Content-Length: ");
    if (content_length_pos == std::string::npos) {
        return HTTP411; 
    }
    std::string content_length_str = headers.substr(content_length_pos + 16);
    content_length_str = content_length_str.substr(0, content_length_str.find("\r\n"));

    int content_length = 0;
    std::istringstream iss(content_length_str);
    iss >> content_length;    

    // Extraer el cuerpo de la solicitud
    std::string body = request.substr(header_end + 4, content_length);

    // Busco en la cabecera donde está el tipo de datos que contiene el cuerpo de la petición, si no está envío un error 415
    std::size_t content_type_pos = headers.find("Content-Type: ");
    if (content_type_pos == std::string::npos) {
        return HTTP415;
    }

    std::string content_type = headers.substr(content_type_pos + 14);
    std::size_t content_type_end = content_type.find("\r\n");
    if (content_type_end != std::string::npos) {
        content_type = content_type.substr(0, content_type_end);
    }

    // Pongo las variables de entorno necesarias por el CGI para ejecutar el script de PHP
    setenv("REQUEST_METHOD", "POST", 1);
    setenv("CONTENT_TYPE", content_type.c_str(), 1);
    setenv("CONTENT_LENGTH", content_length_str.c_str(), 1);
    setenv("SCRIPT_FILENAME", (server_root + request_path).c_str(), 1);
    setenv("GATEWAY_INTERFACE", "CGI/1.1", 1);
    setenv("SERVER_PROTOCOL", "HTTP/1.1", 1);
    setenv("REDIRECT_STATUS", "200", 1); // Esto es necesario para algunas configuraciones de PHP
    setenv("SERVER_SOFTWARE", "CustomServer/0.1", 1);
    setenv("SERVER_NAME", "localhost", 1);
    setenv("SERVER_PORT", "8080", 1);
    setenv("REMOTE_ADDR", "127.0.0.1", 1);

    // Crear los pipes
    int input_pipe[2], output_pipe[2];
    if (pipe(input_pipe) == -1 || pipe(output_pipe) == -1) {
        return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\n\r\nError al crear pipes.";
    }

    pid_t pid = fork();
    if (pid == -1) {
        return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\n\r\nError al hacer fork.";
    } else if (pid == 0) {
        // Código del hijo

        // Redirigir stdin y stdout
        dup2(input_pipe[0], STDIN_FILENO);
        dup2(output_pipe[1], STDOUT_FILENO);

        // Cerrar los extremos no utilizados de los pipes
        close(input_pipe[1]);
        close(output_pipe[0]);

        execl("/usr/bin/php-cgi", "php-cgi", (server_root + request_path).c_str(), NULL);
        exit(1); // Solo se ejecuta si execl falla
    } else {
        // Código del padre

        // Cerrar los extremos no utilizados de los pipes
        close(input_pipe[0]);
        close(output_pipe[1]);

        // Escribir el cuerpo de la solicitud en el pipe de entrada
        write(input_pipe[1], body.c_str(), body.size());
        close(input_pipe[1]); // Ya no necesitamos escribir más

        // Capturar la respuesta del proceso PHP
        char buffer[BUFFER_SIZE];
        std::string response = "HTTP/1.1 200 OK\r\n";

        while (true) {
            ssize_t bytes_read = read(output_pipe[0], buffer, sizeof(buffer));
            if (bytes_read <= 0) {
                break; // No hay más datos
            }
            response.append(buffer, bytes_read);
        }

        close(output_pipe[0]);

        // Esperar a que el hijo termine
        int status;
        waitpid(pid, &status, 0);

        if (status != 0) {
            return "HTTP/1.1 500 Internal Server Error\r\nContent-Type: text/plain\r\n\r\nError al ejecutar PHP.";
        }

        return response;
    }
}



/**
 * @brief Serves the static file that the client request throught the GET request.
 * 
 * @param request_path the path of the file the client requests
 * @param server_root the path were the servers static files are located
 * 
 * @return a std::string of the static file contents.
 */
std::string ServerManager::getFile(std::string request_path, std::string server_root, std::string cgi){
	//Aqui pues saco la ruta absoluta de donde estan las cosas en el server
	std::string path = server_root + request_path;

	//Si al el archivo es .php y el servidor tiene puesto cgi: "php" pues ejecuta el archivo .php que es esencialmente ejecutar un script en la terminal
	// con el comnado php-cgi y te lo devuelve ahi en es STDOUT
	if (path.find(".php") != std::string::npos && !cgi.empty()){
		std::string command = "php-cgi " + path; //Aquie el comando que vamos a ejecutar que es php-cgi 'espacio' el espacio es importante y leugo la ruta donde esta el
		//archivo que quiero ejecutar

		FILE* pipe = popen(command.c_str(), "r"); //popen lo que hace es abrir un proceso para uq ejecute el comando anterior
		if (!pipe)
			return HTTP500;
		
		char buffer[BUFFER_SIZE];
		std::string response = "HTTP/1.1 200 OK\r\n"; //cabecera de la respuesta

		while (fgets(buffer, sizeof(buffer), pipe) != NULL) //Vamos leyendo el resultado de la ejecucion del comando para guardarlo en un string y pasarselo al cliente
			response += buffer;

		pclose(pipe); //cierro el proceso
		//devuelvo la respuesta que en este caso es el archivo .php
		return response;
	}

	int fd = open(path.c_str(), O_RDONLY); //abro el archivo normal si no puedo leerlo mando error 400
	if (fd < 0){
		return HTTP400;
	}

	char buffer[BUFFER_SIZE];
	std::string response = "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n"; //cabecera de la respuesta
	ssize_t bytes;
	while ((bytes = read(fd, buffer, sizeof(buffer))) > 0){//Vamos leyendo los contenido del archivo y metiendolos en las respuestas 
		response.append(buffer, bytes);
	}
	
	close(fd); //cierro el archivo
	return response; //devuelvo la respuesta.
}

/**
 * @brief handles GET, POST and DELETE HTTP requests.
 * 
 * @param request The incoming request from the client.
 * @param server_conf The configuration of the server that was requested.
 * 
 * @return on success returns the request on failure returns 405
 */
std::string ServerManager::handle_request(std::string const request, ConfigParser::Server server_conf){
	std::istringstream req_stream(request);
	std::string method, path, protocol;
	size_t index = 0;
	//Esta va ser la funcon principal del manejor de peticiones
	//Esto de aqui abajo parece complejo pero es simple y comodo, basicmanete meto la string request en un stream que es como si fuera un archivo
	//Esto me permite coger cosas por palabaras, cada vez que hago un >> pillo una palabra.
	req_stream >> method >> path >> protocol;

	//Aqui pillo la configuracion de la ruta en la que se ha hecho la request.
	//Por si no lo sabias para eso sirve locations, para hacer configuraciones especificas de lugares especificos del servidor, por ejemplo no permitir que se haga
	//peticiones GET en cierta rutas.
	for (size_t i = 0; i < server_conf.locations.size(); i++){
		if (server_conf.locations[i].path == path){
			index = i;
			break;
		}
	}
	//Aqui compruebo que si no hay limits se ponga "NONE", para manejar restricciones
	if (server_conf.locations[index].limits.empty())
		server_conf.locations[index].limits.push_back("NONE");
	//El metodo GET basicamente te devuelve archivos, ya sean dinamicos o estaticos y pues eso hago aqui, pillo q archivo me piden y se lo devuelvo
	if (method == "GET"){
		//Compruebo que se pueda hacer GET en la ruta desde la q se pidio si no pues no se hace y se devuelve un error 405
		if (server_conf.locations[index].limits[0] == "NONE" || !this->checkLimits(server_conf.locations[index].limits, "GET")){
			if (path == "/") //Si el path es el raiz devuelvo el index que tenga el locations por defecto
				path = "/" + server_conf.locations[index].index;
			return getFile(path, server_conf.locations[index].root, server_conf.cgi); //Aqui es donde sucede toda la magia del archivo 
		}
		return HTTP405;
	//El metodo POST hace muchas cosas de momento lo que tengo hecho 100% es subir archivos al servidor lo otro esta a casi.
	} else if (method == "POST"){
		//Compruebo que se pueda hacer POST en la ruta desde la q se pidio si no pues no se hace y devuelve un error 405
		if (server_conf.locations[index].limits[0] == "NONE" || !this->checkLimits(server_conf.locations[index].limits, "GET")){
			//Si se hace un upload pues es que se quiere subir un archivo al servidro entonces puese eso hacemos
			if (path == "/upload") {
				return handlePostUpload(request, server_conf.locations[index].root); // MAGIC este no te lo comento por dentro porque funciona perfe asi que no creo q lo necesites tocar
			}
			//Esto se supone que tiene que poder hacer es que tiene que coger los datos de un formulario html y pues mandarselos a un codigo php y q haga cosas chulas
			//Ejemplo del html es el archivo process.php en la carpeta explcio mas en detalle lo que tiene que hacer alli.
			return handlePost(request, path, server_conf.locations[index].root); // mas magia pero no funciona del todo
		}
		return HTTP405;
	//EL metodo DELETE no se ni que hace no lo he mirado jsjsjsj :3
	} else if (method == "DELETE"){
		//ejecutar DELETE
		return "HTTP/1.1 200 OK\r\nContent-Type: text/html\r\n\r\n<h1>200 DELETE/h1>";
	}
	//Si no es ningun metodo pues error 405 y pa lante como los de alicante
	return HTTP405;
}

/**
 * @brief Starts the server so it can handle request and connections.
 */
void ServerManager::startServer(){
	//Creo mi estructura de pollfd y meto el socket del servidor
	std::vector<struct pollfd> fds;
	struct pollfd server_pollfd = {this->server_fd, POLLIN, 0};
    fds.push_back(server_pollfd);

	//manejo las señales como en minishell para que el ctrl-C no cierre el programa a cascoporro
	signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

	//Bucle principal
	while (running) {
		int count = poll(&fds[0], fds.size(), -1); // esto comprueba que hay eventos en los file descriptors
		//&fds[0] Es un puntero al primer elemento del vector de estructuras pollfd
		//fds.size() Es el numero de elementos del vector
		//-1 indica el tiempo que esperara poll como el valor es -1 esperara indefinidamente
		if (!running) {
			// si se ha presionado el ctrl-C sale del bucle o exit.
			std::cout << "Exiting server...\n";
			break;
		}	
		//si hay un error con poll Salgo
		if (count < 0){
			std::cerr << "Error en poll: ";
			perror("poll");
			break;
		}
		//bucle de conexiones
		for (size_t i = 0; i < fds.size(); i++){
			if (fds[i].revents & POLLIN){
				if (fds[i].fd == this->server_fd){ // Si el evento sucede en el fd del servidor es que un cliente se quiere conectar, entonces lo conectamos
					//Nueva conexion
					//Hasta aqui creo q llegaste tu en el tuyo pero bueno lo explico que no hace daño :D
					struct sockaddr_in client; // Declaro una estructura sockaddr_in -> Esta estructura se utiliza para alamacenar la dirección
					//del cliente que se conecta al servidor, incluye informacion como la direccion IP y el puerto del cliente
					socklen_t client_len = sizeof(client); // Esto lo uso para indicar el tamaño de la estructura anterio poco mas la verdad, la chicha
					//esta en el accept
					int client_fd = accept(this->server_fd, (struct sockaddr*)&client, &client_len); //accept lo que hace es que espera a una conexion entrante
					// en el socket del servidor (server_fd), Si hay una conexion entrante la funcion crea un nuevo socket para manejar la conexion con el clienta
					//también llena la estructura sockaddr_in que creamos anteriormente y actualiza client_len, en resume es la repolla y hace un monton de cosas por detrás
					//que asi nosotros no tenemos que hacer y nos devuelve el file descriptor del cliente
					if (client_fd >= 0){ // Si el accept no falla meteremos al cliente en nuestro vector para manejarlo
						struct pollfd poll_client = {client_fd, POLLIN, 0};
						fds.push_back(poll_client);
						//mensaje de exito
						std::cout << "Cliente conectado: " << client_fd << std::endl;
					} else { // Si falla pues mensaje de error y tirando
						//mensaje de error
						std::cerr << "No se pudo conectar el cliente: " << client_fd << std::endl;
					}	
				} else {
					//Manejar cliente
					//Aqui hay un monton de cosas del protocolo HTTP, es decir todo.
					//Asi de manera resumen un servidor HTTP va a recibir una peticion contestar y luego corta la conexion establecida
					//Entonces por partes vamos a desglosar la mierda esta que he hecho mientras te lo pasabas de puta madre por el caribe.
					char buffer[BUFFER_SIZE];
					std::memset(buffer, 0, sizeof(buffer));
					ssize_t bytes = read(fds[i].fd, buffer, sizeof(buffer)); //Leeo la peticion entera enciada por el cliente
					if (bytes > 0){ //Si bien la manejamos
						ConfigParser::Server server_conf = getServerName(std::string(buffer, bytes)); // Esto es lo que mencione de variios servidores escucharndo
						//Al mismo host:port basicamente pillo la configuracion del servidor que llama el cliente
						std::string const response = handle_request(std::string(buffer, bytes), server_conf); // Y aqui pasa la magia
						send(fds[i].fd, response.c_str(), strlen(response.c_str()), 0);// Aqui Envio la repuesta al cliente
						//Desconectamos al cliente.
						std::cout << "Client disconnected: " << fds[i].fd << std::endl;
						close(fds[i].fd);
						fds.erase(fds.begin() + i);
						--i;
					} else { //Si mal pues cortamos y tiramos
						// Desconexion o error.
						std::cout << "Client disconnected: " << fds[i].fd << std::endl;
						close(fds[i].fd);
						fds.erase(fds.begin() + i);
						--i;
					}
				}
			}
		}
	}
	//Cieroo los sockets
	close(fds[0].fd);
	close(this->server_fd);
}

ConfigParser::Server ServerManager::getServerName(std::string request){
	std::istringstream req_stream(request);
	std::string protocol, host, temp;
        
    std::getline(req_stream, temp, '/');
    std::getline(req_stream, host, '/');

	for (size_t i = 0; i < this->server_confs.size(); i++){
		if (this->server_confs[i].server_name == host)
			return this->server_confs[i];
	}
	return this->server_confs[0];
}

/**
 * @brief Gets the private attribute server_confs.
 * 
 * @return returns a vector<ConfigParser::Server> with the server_confs of the object.
 */
ConfigParser::Server ServerManager::getServersConf(){
	return this->server_confs[0];
}

/**
 * @brief Adds a server conf to the ServerManager object.
 * 
 * @param server_conf The server configuration that is going to be added.
 */
void ServerManager::addConf(ConfigParser::Server server_conf){
	this->server_confs.push_back(server_conf);
}

/**
 * @brief Checks if the method is in the limits vector.
 * 
 * @param limits The limits vector for the location in question.
 * @param search The method that is going to be checked.
 * 
 * @return If the method is found true, otherwise false.
 */
int ServerManager::checkLimits(std::vector<std::string> limits, std::string search) const{
	int exit = 0;
	for (size_t i = 0; i < limits.size(); i++){
		if (limits[i] == search){
			exit = 1;
		}
	}
	return exit;
}

