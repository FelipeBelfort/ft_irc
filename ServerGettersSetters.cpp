/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerGettersSetters.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 17:06:07 by jm                #+#    #+#             */
/*   Updated: 2024/04/22 20:28:06 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/**
 * @brief returns the indexed socket's file descriptor
 * 
 * @param index index of the pollfd referring to the actual user in 
 * 			Server's pollfds vector
 * @return int 
 */
int 	Server::getSockfd(const size_t& index)
{
	return (_sockets[index].fd);
}

/**
 * @brief returns the indexed socket's 'revents' field
 * 
 * @param index index of the pollfd referring to the actual user in 
 * 			Server's pollfds vector
 * @return short 
 */
short 	Server::getSockrevents(const size_t& index)
{
	return (_sockets[index].revents);
}

/**
 * @brief returns the date on which the server started
 * 
 * @return const std::string& 
 */
const std::string& 	Server::getStartDate()
{
	return (_startDate);
}

/**
 * @brief returns the number of users registered
 * 
 * @return const size_t 
 */
size_t		Server::getNbOfUsers(void)
{
	// TODO complete
	return (Server::_clients.size());
}

size_t		Server::getMaxOfUsers(void)
{
	// TODO complete
	return (0);
}

size_t		Server::getNbOfInvUsers(void)
{
	// TODO complete
	return (0);
}

size_t		Server::getNbOfOtherServs(void)
{
	// TODO complete
	return (0);
}

size_t		Server::getNbOfOperators(void)
{
	// TODO complete
	return (0);
}

size_t					Server::getNbOfUnkConnect(void)
{
	// TODO complete
	return (0);
}

size_t					Server::getNbOfChannels(void)
{
	// TODO complete
	return (0);
}

const std::string		Server::getMsgOfTheDay(void)
{
	// TODO complete
	return ("");
}
