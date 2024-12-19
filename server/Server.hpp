/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:30 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/19 08:27:59 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "../client/Client.hpp"

class Server
{
	private:
		int	socketfd;							// Socket principal del servidor
		std::vector<pollfd> fds;				// Lista de descriptores de archivo (poll)
		std::vector<Client> clients;			// Clientes conectados

		void	setupSocket(Config &config);	// Configurar el socket del servidor
		void	acceptNewClient();				// Aceptar Nuevas peticiones
		void	handleClient(int clientIndex, Config &config);	// Manejar las peticiones del cliente
	
	public:
		Server(Config &config);
		~Server();

		void run(Config &config);								//bucle principal del servidor.
};

#endif