/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:53:35 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/13 15:10:33 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP

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

class Config
{
	private:
		int	port;
		std::string host;
		void	loadConfig(const std::string &file);
		
	public:
		Config(const std::string &file);
		~Config();

		int	getPort() const;
		std::string getHost() const;
};

#endif