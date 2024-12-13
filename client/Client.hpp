/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:33 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/13 15:05:39 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "config/Config.hpp"

class Client
{
	private:
		int	socketClient;	// Socket del cliente

	public:
		Client(int socket);
		~Client();
};

#endif