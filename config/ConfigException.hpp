/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ConfigException.hpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 11:29:24 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/19 11:48:00 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_EXCEPTION_HPP
#define CONFIG_EXCEPTION_HPP

# include <exception>
# include <string>
# include <sstream>

class ConfigException : public std::exception {
private:
    std::string message;

public:
    ConfigException(const std::string& msg) : message(msg) {}
    virtual ~ConfigException() throw(){}

    template <typename T>
    ConfigException(const char* format, const T& arg) {
        std::ostringstream oss;
        oss << format << arg;
        message = oss.str();
    }

    template <typename T, typename U>
    ConfigException(const char* format, const T& arg1, const U& arg2) {
        std::ostringstream oss;
        oss << format << arg1 << arg2;
        message = oss.str();
    }

    // Cambia la firma de 'what' para que coincida con std::exception
    virtual const char* what() const throw() {  // Usar 'noexcept'
        return message.c_str();
    }
};

#endif