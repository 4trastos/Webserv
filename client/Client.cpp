/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:42 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/18 15:54:50 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../client/Client.hpp"
#include "../config/Config.hpp"
#include <unistd.h>
#include <netdb.h> 

Client::Client(int socket): socketClient(socket){}

Client::~Client(){
	close(socketClient);
}