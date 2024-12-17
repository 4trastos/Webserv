/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:45 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/17 17:59:51 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../config/Config.hpp"
#include "../server/Server.hpp"
#include "../client/Client.hpp"
#include <cstdlib>

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

std::string Config::getIndex() const{
	return (index);
}

std::string Config::getRoot() const{
	return (root);
}

void	Config::loadConfig(const std::string &file){
	std::ifstream configFile(file.c_str());
	if (!configFile.is_open())
		std::cout << "Error al abrir archivo de configuración" << std::endl;

	std::string line;
	while (std::getline(configFile, line))
	{
		if (line.find("listen") != std::string::npos)
		{
			std::string portStr = line.substr(line.find(":") + 1);
			this->port = atoi(portStr.c_str());
		}
		else if (line.find("host") != std::string::npos)
			this->host = line.substr(line.find(":") + 1);
		else if (line.find("root") != std::string::npos)
			this->root = line.substr(line.find(":") + 1);
		else if (line.find("index") != std::string::npos)
			this->root = line.substr(line.find(":") + 1);
	}
	configFile.close();
}

