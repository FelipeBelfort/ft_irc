/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerGettersSetters.cpp                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfaye <jfaye@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/21 17:06:07 by jm                #+#    #+#             */
/*   Updated: 2024/05/31 17:38:38 by jfaye            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int		Server::getIndex(const std::string& nick)
{
	User*	_user;

	_user = NULL;
	for (size_t i = 1; i < _sockets.size(); i++)
	{
		_user = getUser(i);
		if (!_user)
			return (-1);
		if (_user->getNickname() == Libftpp::strToLower(nick))
			return (i);
	}
	return (-1);
}

User* 	Server::getUser(const std::string& nick)
{
	if (_users.empty())
		return (NULL);
	for (std::map<int, User>::iterator it = _users.begin(); \
		it != _users.end(); it++)
	{
		if ((*it).second.getNickname() == Libftpp::strToLower(nick))
			return (&(*it).second);
	}
	return (NULL);
}

/**
 * @brief 
 * 
 * @param index index of the user's socket in sockets container
 * @return User* or NULL if failed to find a user bound to INDEX 
 */
User* 	Server::getUser(const size_t& index)
{
	User*	ptr;

	ptr = NULL;
	try
	{
		std::map<int, User>::iterator it = _users.find(getSockfd(index));
		if (it != _users.end())
			return (&(*it).second);
	}
	catch(const std::exception& e)
	{
		std::cerr << "Error: getUser() failure: " << e.what() << '\n';
	}
	return (NULL);
}

size_t	 	Server::getSocketsSize(void)
{
	return (_sockets.size());
}

/**
 * @brief returns the indexed socket's file descriptor
 * 
 * @param index index of the pollfd referring to the actual user in 
 * 			Server's pollfds vector
 * @return int 
 */
const int& 	Server::getSockfd(const size_t& index)
{
	if (index < _sockets.size())
		return (_sockets[index].fd);
	throw (UserNotFoundException());
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
	if (index < _sockets.size())
		return (_sockets[index].revents);
	return (_error);
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
	return (Server::_users.size());
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
	return (channels.size());
}

const std::string		Server::getMsgOfTheDay(void)
{
	// TODO complete
	return ("");
}
