/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   incl.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davgalle <davgalle@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 10:12:51 by davgalle          #+#    #+#             */
/*   Updated: 2024/12/19 10:13:37 by davgalle         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef INCL_HPP
# define INCL_HPP

# include <cctype>
# include <string>
# include <iostream>
# include <fstream>
# include <cstdlib>

std::string trim_left(const std::string& str);
std::string trim_right(const std::string& str);
std::string trim(const std::string& str);

#endif