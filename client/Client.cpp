/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:42 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/17 13:30:31 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../client/Client.hpp"
#include "../config/Config.hpp"
#include <unistd.h>

Client::Client(int socket): socketClient(socket){}

Client::~Client(){
	close(socketClient);
}