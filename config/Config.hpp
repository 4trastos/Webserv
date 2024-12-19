/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:35 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/19 11:28:56 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

# include "ConfigException.hpp"
# include <string>
# include <iosfwd>
# include <iomanip>
# include <map>
# include <fstream>
# include <stdexcept>
# include <cstring>
# include <vector>
# include <iostream>
# include <poll.h>

struct LocationConfig {
    std::string path;
    std::string root;
    std::string index;
};

struct ServerConfig {
    int listen;
    std::string server_name;
    std::string host;
    std::string root;
    std::vector<LocationConfig> locations;
};

class Config
{
	private:
		std::vector<ServerConfig> servers;			// Ahora almacena un vector de servidores
    	void loadConfig(const std::string &file);
		
	public:
		Config(const std::string &file);
		~Config();

		// Métodos para acceder a la configuración (ahora para múltiples servidores)
		const std::vector<ServerConfig>& getServers() const;
		
		// Métodos para acceder a la configuración de un servidor específico (por ejemplo, el primero)
		int getPort(size_t serverIndex = 0) const;
    	std::string getHost(size_t serverIndex = 0) const;
    	std::string getRoot(size_t serverIndex = 0) const;
    	std::string getIndex(size_t serverIndex = 0, size_t locationIndex = 0) const;
    	const LocationConfig& getLocation(size_t serverIndex = 0, size_t locationIndex = 0) const;
    	size_t getServerCount() const;
    	size_t getLocationCount(size_t serverIndex = 0) const;
};
#endif