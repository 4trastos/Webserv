/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:40 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/17 17:48:33 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>
#include <sstream>

Server::Server(Config &config) {
	setupSocket(config);	
}

Server::~Server(){
	close(socketOne);
}

void	Server::setupSocket(Config &config){
	// crear el socket
	socketOne = socket(AF_INET, SOCK_STREAM, 0);
	if (socketOne < 0)
		std::cout << "Error al crear el socket" << std::endl;

	// Configurar el socket no bloqueante
	fcntl(socketOne, F_SETFL, O_NONBLOCK);
	
	// Asignar el socket a una dirección
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(config.getPort());
	address.sin_addr.s_addr = inet_addr(config.getHost().c_str());
	
	// Enlazar el socket
	if (bind(socketOne, (struct sockaddr *)&address, sizeof(address)) < 0)
		std::cout << "Error bind" << std::endl;
	
	// Escuchar conexiones entrantes
	if (listen(socketOne, SOMAXCONN) < 0)
		std::cout << "Error en listen" << std::endl;
	// Agregar el socket a la lista de descriptores de 'poll'
	std::cout << "Socket creado y escuchando en " << config.getHost() << ":" << config.getPort() << " con fd:" << socketOne << std::endl;
	pollfd serverPollfd = {socketOne, POLLIN, 0};
	this->fds.push_back(serverPollfd);
}

void	Server::acceptNewClient()
{
	int clientSocket = accept(socketOne, NULL, NULL);
	if (clientSocket < 0)
		return;

	// Configurar el nuevo cliente como no bloqueante
	fcntl(clientSocket, F_SETFL, O_NONBLOCK);
	
	// Agregar el nuevo cliente a poll
	struct pollfd clientPollfd = {clientSocket, POLLIN, 0};
	this->fds.push_back(clientPollfd);
	this->clients.push_back(Client(clientSocket));
}

void	Server::handleClient(int clientIndex)
{
	char buffer[1024];
	int	bytesRead = recv(fds[clientIndex].fd, buffer, sizeof(buffer), 0);

	if (bytesRead < 0)
	{
		close(fds[clientIndex].fd);
		fds.erase(fds.begin() + clientIndex);
		clients.erase(clients.begin() + clientIndex - 1);
	}
	else
	{
		std::cout << "Datos recibidos: " << buffer << std::endl;

		// Simplemente procesamos cualquier petición con "GET /"
		std::string request(buffer);
		if (request.find("GET /") != std::string::npos)
		{
			std::ifstream file("../html/index.html");
			if (!file)
			{
				std::string error404 = "HTTP/1.1 404 Not Found\r\nContent-Length: 13\r\n\r\n404 Not Found";
				send(fds[clientIndex].fd, error404.c_str(), error404.size(), 0);
			}
			else
			{
				// Leer el contenido del archivo HTML
				std::stringstream htmlContent;
				htmlContent << file.rdbuf();
				file.close();

				std::string content = htmlContent.str();

				// Construir y enviar la respuesta HTTP
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

void	Server::run()
{
	std::cout << "dentro de run: " << std::endl;
	std::cout << "Tamaño de fds: " << fds.size() << std::endl;
	std::cout << "Socket principal (socketOne): " << socketOne << std::endl;

	while (true)
	{
		// Esperar conexiones o datos con poll
		int pollCount = poll(fds.data(), fds.size(), -1);
		std::cout << "dentro antes del for: " << std::endl;
		if (pollCount < 0)
			throw std::runtime_error("Error en poll");
		for (size_t i = 0; i < fds.size(); i++)
		{
			std::cout << "dentro de for: " << std::endl;
			if (fds[i].revents & POLLIN)
			{
				if (fds[i].fd == socketOne){
					std::cout << "Cliente conectado: " << std::endl;
					acceptNewClient();	// Nueva conexión
				}
				else{
					handleClient(i);	// Peticiones de clientes
				}
			}
		}
			
	}
}
