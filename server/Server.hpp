/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:30 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/13 15:49:38 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include "config/Config.hpp"
# include "client/Client.hpp"

class Server
{
	private:
		int	socketOne;							// Socket principal del servidor
		std::vector<pollfd> fds;				// Lista de descriptores de archivo (poll)
		std::vector<Client> clients;			// Clientes conectados

		void	setupSocket(Config &config);	// Configurar el socket del servidor
		void	acceptNewClient();				// Aceptar Nuevas peticiones
		void	handleClient(int clientIndex);	// Manejar las peticiones del cliente
	
	public:
		Server(Config &config);
		~Server();

		void run();	//bucle principal del servidor.
};

#endif