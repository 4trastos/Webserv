/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:45 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/13 15:43:14 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config/Config.hpp"
#include "server/Server.hpp"
#include "client/Client.hpp"

Config::Config(const std::string &file){
	loadConfig(file);	
}

Config::~Config(){}

int	Config::getPort() const{
	return (port);
}

std::string	Config::getHost() const{
	return (host);
}

void	Config::loadConfig(const std::string &file){
	std::ifstream configFile(file.c_str());
	if (!configFile.is_open()) throw std::runtime_error("Error al abrir archivo de configuración");

	std::string line;
	while (std::getline(configFile, line))
	{
		if (line.find("port") != std::string::npos)
			port = std::stoi(line.substr(line.find("=") + 1));
		else if (line.find("host") != std::string::npos)
			host = line.substr(line.find("=") + 1);
	}
		
}

