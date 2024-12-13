/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:40 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/13 17:01:10 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <cstring>

Server::Server(Config &config) {
	setupSocket(config);	
}

Server::~Server(){
	close(socketOne);
}

void	Server::setupSocket(Config &config){
	// crear el socket
	socketOne = socket(AF_INET, SOCK_STREAM, 0);
	if (socketOne < 0) throw std::runtime_error("Error al crear el socket");

	// Configurar el socket no bloqueante
	fcntl(socketOne, F_SETFL, O_NONBLOCK);
	
	// Asignar el socket
	sockaddr_in address;
	address.sin_family = AF_INET;
	address.sin_port = htons(config.getPort());
	address.sin_addr.s_addr = inet_addr(config.getHost().c_str());
	
	// Enlazar el socket
	// Escuchar conexiones entrantes
	// Agregar el socket a la lista de descriptores de poll
}


