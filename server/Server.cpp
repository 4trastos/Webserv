/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:40 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/19 12:48:33 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sstream>
#include <netdb.h> 

Server::Server(Config &config) {
	setupSocket(config);	
}

Server::~Server(){
	if (socketfd >= 0)
		close(socketfd);
}

template <typename T>
std::string to_string(T value){
	std::stringstream ss;
	ss << value;
	return ss.str();
}

void	Server::setupSocket(Config &config){
	struct addrinfo hints, *res, *head;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC; 	 	// IPv4 o IPv6
	hints.ai_socktype = SOCK_STREAM;	// tipo de socket
	hints.ai_flags = AI_PASSIVE;		// Usar la IP de la máquina

	std::string portStr = to_string(config.getPort());

	// Obtener una lista de direcciones posibles
	int status = getaddrinfo(config.getHost().c_str(), portStr.c_str(), &hints, &res);
	if (status != 0){
		std::cout << "Error en getaddrinfo: " << gai_strerror(status) << std::endl;
        return;
	}
	
	head = res;
	//int socketfd;
	while (res != NULL)
	{
		socketfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if (socketfd < 0)
			std::cerr << "Error: no se pudo crear el socket" << std::endl;
		else {
            // Configurar SO_LINGER para evitar TIME_WAIT
            struct linger sl;
            sl.l_onoff = 1;  // Activar linger
            sl.l_linger = 0; // Cerrar el socket inmediatamente
            if (setsockopt(socketfd, SOL_SOCKET, SO_LINGER, &sl, sizeof(sl)) < 0) {
                std::cerr << "Error en setsockopt (SO_LINGER)" << std::endl;
                close(socketfd);
                res = res->ai_next;
                continue;
            }
			if (bind(socketfd, res->ai_addr, res->ai_addrlen) == 0)
			{
				std::cout << "Socket creado y enlazado en " << config.getHost() << ":" << config.getPort() << std::endl;
            	break;
			}
		}
		close(socketfd);
		res = res->ai_next;
	}

	// Si no se encontró ninguna dirección válida
	if (res == NULL) {
		std::cout << "Error: no se pudo enlazar el socket a ninguna dirección" << std::endl;
		std::cerr << "Error al enlazar el socket: " << strerror(errno) << std::endl;
		freeaddrinfo(head);
		return;
	}
	
	freeaddrinfo(head);

	// Configurar SO_REUSEADDR para reutilizar el puerto
	int opt = 1;
	if (setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
		std::cerr << "Error en setsockopt" << std::endl;
		close(socketfd);
		return;
	}

	// Configurar el socket como no bloqueante
	fcntl(socketfd, F_SETFL, O_NONBLOCK);

	std::cout << "Socket escuchando en " << config.getHost() << ":" << config.getPort() << " con fd: " << socketfd << std::endl;

	// Agregar el socket a la lista de descriptores de 'poll'
	pollfd serverPollfd = {socketfd, POLLIN, 0};
	this->fds.push_back(serverPollfd);
}

void	Server::acceptNewClient()
{
	int clientSocket = accept(socketfd, NULL, NULL);
	if (clientSocket < 0){
		std::cout << "Error al aceptar nueva conexión" << std::endl;
		return;
	}

	// Configurar el nuevo cliente como no bloqueante
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	
	// Agregar el nuevo cliente a poll
	struct pollfd clientPollfd = {clientSocket, POLLIN, 0};
	this->fds.push_back(clientPollfd);
	std::cout << "Nuevo cliente conectado, fd: " << clientSocket << std::endl;
}

void	Server::handleClient(int clientIndex, Config &config)
{
	std::cout << "Leyendo las solicitudes" << std::endl;
	char buffer[1024];
	int	bytesRead = recv(fds[clientIndex].fd, buffer, sizeof(buffer), 0);

	if (bytesRead <= 0)
	{
		close(fds[clientIndex].fd);
		fds.erase(fds.begin() + clientIndex);
		clients.erase(clients.begin() + clientIndex - 1);
	}
	else
	{
        buffer[bytesRead] = '\0';
        std::cout << "Petición recibida: " << buffer << std::endl;

        std::string request(buffer);
        if (request.find("GET /") != std::string::npos) {
            std::string filePath = config.getRoot() + "/" + config.getIndex();

            std::ifstream file(filePath.c_str());
            if (!file) {
                std::string error404 = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
                send(fds[clientIndex].fd, error404.c_str(), error404.size(), 0);
            }
			else {
                std::stringstream htmlContent;
                htmlContent << file.rdbuf();
                file.close();

                std::string content = htmlContent.str();

                std::stringstream response;
                response << "HTTP/1.1 200 OK\r\n";
                response << "Content-Length: " << content.size() << "\r\n";
                response << "Content-Type: text/html\r\n";
                response << "\r\n";
                response << content;

                std::string responseStr = response.str();
                send(fds[clientIndex].fd, responseStr.c_str(), responseStr.size(), 0);
            }
        }
    }
}

void	Server::run(Config &config)
{
	while (true)
	{
		int pollCount = poll(fds.data(), fds.size(), -1);
		if (pollCount < 0)
			std::cerr << "Error en poll: " << strerror(errno) << std::endl;
		
		std::cout << "Poll detectó actividad en " << pollCount << " fds" << std::endl;
		
		for (size_t i = 0; i < fds.size(); i++)
		{
			std::cout << "Revisando fd: " << fds[i].fd << ", revents: " << fds[i].revents << std::endl;
			if (fds[i].revents & POLLIN)
			{
				std::cout << "dentro" << std::endl;
				if (fds[i].fd == socketfd){
					std::cout << "Cliente conectado: " << std::endl;
					acceptNewClient();
				}
				else{
					handleClient(i, config);
				}
			}
		}
			
	}
}
