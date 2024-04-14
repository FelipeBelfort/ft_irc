/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Error.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: TheTerror <jfaye@student.42lyon.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/26 16:37:17 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/11 21:19:52 by TheTerror        ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Libftpp.hpp"

bool			Libftpp::error(const std::string& msg)
{
	std::cerr << "Error: " << msg << std::endl;
	return (false);
}

bool			Libftpp::ft_perror(const std::string& msg)
{
	std::cerr << "Error: ";
	std::perror(msg.c_str());
	return (false);
}
