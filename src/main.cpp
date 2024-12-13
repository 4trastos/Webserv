/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:52:00 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/13 15:18:50 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server/Server.hpp"
#include "client/Client.hpp"
#include "config/Config.hpp"
#include "librs.h"

int	main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cout << "Error: Invalid number of arguments (Only 1 config file needed)" << std::endl;
		return (1);
	}
	Config config(argv[1]);
	Server server(config);

	try
	{
		server.run();
	}
	catch(const std::exception& e)
	{
		std::cerr << "Server error: "<< e.what() << '\n';
		return (1);
	}

	return (0);
}