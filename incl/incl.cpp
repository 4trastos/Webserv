/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   incl.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:09:21 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/19 10:11:20 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <cctype>
#include <string>
#include <iostream>
#include <fstream>
#include <cstdlib>

std::string trim_left(const std::string& str) {
    std::string result = str;
    size_t startpos = result.find_first_not_of(" \t\n\r"); // Busca el primer no espacio
    if (std::string::npos != startpos) {
        result = result.substr(startpos);
    } else {
        result = ""; // Si solo hay espacios, devuelve un string vacío
    }
    return result;
}

std::string trim_right(const std::string& str) {
    std::string result = str;
    size_t endpos = result.find_last_not_of(" \t\n\r"); // Busca el último no espacio
    if (std::string::npos != endpos) {
        result = result.substr(0, endpos + 1);
    } else {
        result = ""; // Si solo hay espacios, devuelve un string vacío
    }
    return result;
}

std::string trim(const std::string& str) {
    return trim_right(trim_left(str));
}
