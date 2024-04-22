/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:37:17 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/21 19:27:21 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Libftpp.hpp"

/**
 * @brief print error message on the standard error stream
 * 
 * @param msg error message
 * @return false 
 */
bool			Libftpp::error(const std::string& msg)
{
	std::cerr << "Error: " << msg << std::endl;
	return (false);
}

/**
 * @brief print 'Error' then call std::perror to describe 
 * 		the value of 'errno'
 * 
 * @param msg message passed to std::perror function
 * @return false 
 */
bool			Libftpp::ft_perror(const std::string& msg)
{
	std::cerr << "Error: ";
	std::perror(msg.c_str());
	return (false);
}
