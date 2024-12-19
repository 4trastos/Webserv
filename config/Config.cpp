/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:45 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/19 11:18:29 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../config/Config.hpp"
#include "../server/Server.hpp"
#include "../client/Client.hpp"
#include "../incl/incl.hpp"
#include <cstdlib>
#include <netdb.h> 

Config::Config(const std::string &file){
	loadConfig(file);	
}

Config::~Config(){}

std::vector<ServerConfig> parseConfig(const std::string& filename) {
    std::vector<ServerConfig> servers;
    std::ifstream configFile(filename.c_str());
    std::string line;

    if (!configFile.is_open()) {
        std::cerr << "Error al abrir el archivo de configuración: " << filename << std::endl;
        return servers;
    }

    ServerConfig currentServer;
    LocationConfig currentLocation;
    bool inLocationBlock = false;

    while (std::getline(configFile, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line == "SERVER") {
            currentServer = ServerConfig();
        }
        else if (line == "CLOSESERVER") {
            servers.push_back(currentServer);
        }
        else if (line.find("LOCATION:") == 0) {
            inLocationBlock = true;
            currentLocation = LocationConfig();
            currentLocation.path = trim(line.substr(line.find(":") + 1));
        }
        else if (line == "CLOSELOCATION") {
            inLocationBlock = false;
            currentServer.locations.push_back(currentLocation);
        }
        else {
            std::string key = line.substr(0, line.find(":"));
            key = trim(key);
            std::string value = line.substr(line.find(":") + 1);
            value = trim(value);

            if (inLocationBlock) {
                if (key == "root") currentLocation.root = value;
                else if (key == "index") currentLocation.index = value;
            }
            else {
                if (key == "listen") currentServer.listen = std::atoi(value.c_str());
                else if (key == "server_name") currentServer.server_name = value;
                else if (key == "host") currentServer.host = value;
                else if (key == "root") currentServer.root = value;
            }
        }
    }

    configFile.close();
    return servers;
}

void Config::loadConfig(const std::string &file) {
    servers = parseConfig(file);
}

const std::vector<ServerConfig>& Config::getServers() const {
    return servers;
}

int Config::getPort(size_t serverIndex) const {
    if (serverIndex < servers.size()) {
        return servers[serverIndex].listen;
    }
    return 0; // O un valor por defecto/excepción
}

std::string Config::getHost(size_t serverIndex) const {
    if (serverIndex < servers.size()) {
        return servers[serverIndex].host;
    }
    return ""; // O un valor por defecto/excepción
}

std::string Config::getRoot(size_t serverIndex) const {
    if (serverIndex < servers.size()) {
        return servers[serverIndex].root;
    }
    return ""; // O un valor por defecto/excepción
}

std::string Config::getIndex(size_t serverIndex, size_t locationIndex) const {
    if (serverIndex < servers.size() && locationIndex < servers[serverIndex].locations.size())
    {
        return servers[serverIndex].locations[locationIndex].index;
    }
    return "";
}

const LocationConfig& Config::getLocation(size_t serverIndex, size_t locationIndex) const
{
    if (serverIndex < servers.size() && locationIndex < servers[serverIndex].locations.size())
    {
        return servers[serverIndex].locations[locationIndex];
    }
    static LocationConfig emptyLocation;
    return emptyLocation;
}

size_t Config::getServerCount() const {
    return servers.size();
}

size_t Config::getLocationCount(size_t serverIndex) const {
    if (serverIndex < servers.size())
    {
        return servers[serverIndex].locations.size();
    }
    return 0;
}


/* void	Config::loadConfig(const std::string &file){
	std::ifstream configFile(file.c_str());
	if (!configFile.is_open())
		std::cout << "Error al abrir archivo de configuración" << std::endl;

	std::string line;
	while (std::getline(configFile, line))
	{
		line = trim(line);
		if (line.empty() || line[0] == '#') continue;
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
			this->index = line.substr(line.find(":") + 1);
	}
	configFile.close();
	std::cout << "Configuración cargada:\n";
	std::cout << "Host: " << this->host << "\n";
	std::cout << "Port: " << this->port << "\n";
	std::cout << "Root: " << this->root << "\n";
	std::cout << "Index: " << this->index << "\n";
}
 */
