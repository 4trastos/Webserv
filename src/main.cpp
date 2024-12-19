/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 11:52:00 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/19 11:37:51 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server/Server.hpp"
#include "../parse/ConfigParser.hpp"
#include "../config/ConfigException.hpp"
#include "librs.h"
#include <iostream>
#include <exception>

int main(int argc, char **argv) {
    if (argc != 2) {
        std::cerr << "Error: Número inválido de argumentos. Se necesita un archivo de configuración." << std::endl;
        std::cerr << "Uso: " << argv[0] << " <archivo_de_configuración>" << std::endl;
        return 1; 
    }

    std::cout << "Cargando configuración desde: " << argv[1] << std::endl;

    try {
        Config config(argv[1]);
        Server server(config);

        std::cout << "Servidor iniciado." << std::endl;

        server.run(config);

    } catch (const ConfigException& e) { 
        std::cerr << "Error de configuración: " << e.what() << std::endl;
        return 2; 
    } catch (const std::runtime_error& e) {
        std::cerr << "Error durante la ejecución del servidor: " << e.what() << std::endl;
        return 3;
    } catch (const std::exception& e) {
        std::cerr << "Error inesperado en el servidor: " << e.what() << std::endl;
        return 4;
    } catch (...) {
        std::cerr << "Error desconocido en el servidor." << std::endl;
        return 5;
    }

    std::cout << "Servidor finalizado correctamente." << std::endl;
    return 0;
}

/* int	main(int argc, char **argv)
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
		server.run(config);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Server error: "<< e.what() << '\n';
		return (1);
	}

	return (0);
} */