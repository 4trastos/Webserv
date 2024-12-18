/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:52:00 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/17 13:25:58 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ConfigParser.hpp"

/**
 * @brief This is the constructor of the class that is gonna manage the configuration file.
 * @param fileUrl The url of the config file
 */
ConfigParser::ConfigParser(char const* fileUrl): file(fileUrl){
	if (!file.is_open()) {
        std::cerr << "Could not open file" << std::endl;
	}
}

/**
 * @brief This method is going to add a Server struct to the std::list<Server> of this class.
 * @return returns 0 if everything went OK returns 1 if something went wrong.
 */
int ConfigParser::addServerConf(){
	std::string line;
	Server server;

	while (std::getline(this->file, line)){
		this->trim(line);
		if (line.substr(0, 6) == "SERVER"){
			while(std::getline(this->file, line)){
				this->trim(line);
				if (!line.empty()){
					size_t index = line.find("#", 0);
					if (index != std::string::npos){
						line = line.substr(0, index);
					}
					if (line.substr(0, 11) == "CLOSESERVER")
						break;
					if (line.substr(0, 6) == "listen"){
						if (this->checkColon(6, line))
							return 1;
						std::string portNum = line.substr(7);
						this->trim(portNum);
						for (size_t i = 0; i < std::strlen(portNum.c_str()); i++){
							if (!std::isdigit(portNum.c_str()[i])){
								std::cerr << "Error: Non-numeric port in 'listen' configuration" << portNum << std::endl;
							}
						}
						server.port = atoi(portNum.c_str());
					} else if (line.substr(0, 4) == "host"){
						if (this->checkColon(4, line))
							return 1;
						std::string host = line.substr(5);
						this->trim(host);
						server.host = host;
					} else if (line.substr(0, 11) == "server_name"){
						if (this->checkColon(11, line))
							return 1;
						std::string snValue = line.substr(12);
						this->trim(snValue);
						server.server_name = snValue;
					} else if (line.substr(0, 4) == "root"){
						if (this->checkColon(4, line))
							return 1;
						std::string rootValue = line.substr(5);
						this->trim(rootValue);
						server.root = rootValue;
					} else if (line.substr(0, 8) == "LOCATION"){
						Location location;
						if (this->checkColon(8, line))
							return 1;
						std::string path = line.substr(10, (line.length() - 10));
						this->trim(path);
						location.path = path;
						while (std::getline(this->file, line)){
							this->trim(line);
							if (line.substr(0, 13) == "CLOSELOCATION")
								break;
							if (line.substr(0, 4) == "root"){
								if (this->checkColon(4, line))
									return 1;
								std::string root = line.substr(5);
								this->trim(root);
								location.root = root;
							} else if (line.substr(0, 5) == "index") {
								if (this->checkColon(5, line))
									return 1;
								std::string index = line.substr(6);
								this->trim(index);
								location.index = index;
							} else if (line.substr(0, 9) == "autoindex"){
								if (this->checkColon(9, line))
									return 1;
								std::string autoindex = line.substr(10);
								this->trim(autoindex);
								if (autoindex != "on" && autoindex != "off"){
									std::cerr << "Error: Unexpected word " << autoindex << " on line: " << line << std::endl;
									return 1;
								} else if (autoindex == "on"){
									location.autoindex = true;
								} else {
									location.autoindex = false;
								}
							} else if (line.substr(0, 15) == "redirect_target"){
								if (this->checkColon(15, line))
									return 1;
								std::string redirect = line.substr(16);
								this->trim(redirect);
								location.redirect_target = redirect;
							} else if (line.substr(0, 5) == "limit"){
								std::vector<std::string> limits;
								if (this->checkColon(5, line))
									return 1;
								std::string limit = line.substr(6);
								this->trim(limit);
								std::istringstream stream(limit);
								std::string palabra;
								while (stream >> palabra) {
									limits.push_back(palabra);
								}
								location.limits = limits;
							} else if (line.substr(0, 12) == "fastcgi_pass"){
								std::string fastcgi_pass;
								if (this->checkColon(12, line))
									return 1;
								fastcgi_pass = line.substr(13);
								this->trim(fastcgi_pass);
								location.fastcgi_pass = fastcgi_pass;
							} else if (line.substr(0, 13) == "fastcgi_param"){
								std::string id, value;
								if (this->checkColon(13, line))
									return 1;
								std::istringstream fastcgi(line.substr(14));
								fastcgi >> id >> value;
								location.fastcgi_params.insert(std::make_pair(id, value));
							}
						}
						server.locations.push_back(location);
					} else if ( !line.empty() &&line.c_str()[0] != '#'){
						std::cerr << "Unexpected line: " << line << std::endl;
						return 1;
					}
				}
			}
		}
		this->servers.push_back(server);
		server.locations.clear();
	}
	return 0;
}