/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerOtherTools.cpp                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jfaye <jfaye@student.42lyon.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/05/10 17:52:11 by jm                #+#    #+#             */
/*   Updated: 2024/05/30 15:44:56 by jfaye            ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/**
 * @brief 
 * If the poll.revents faces a problem this fonction will be called to close the socket and erase both the User and the 
 * pollfd from the list. 
 * 
 * @param index the index of the revent in the pollfd list
 * @return boolean
 */
bool	Server::closeClient(size_t index)
{
	if (_sockets[index].fd >= 0)
	{
		if (close(_sockets[index].fd) < 0)
			return (Libftpp::ft_perror("close()"));
	}
	if (index)
		_users.erase(_sockets[index].fd);
	_sockets.erase(_sockets.begin() + index);
	return (true);
}

/**
 * @brief creates a new User instance in users container 
 * 		associated to the socket file descriptor returned by accept().
 * 		This latter is also appended to sockets container.
 * @return true, false
 */
bool	Server::createUser(void)
{
	pollfd	user_socket;

	user_socket.fd = accept(_sockets[0].fd, NULL, NULL);
	if (user_socket.fd == -1)
		return (Libftpp::ft_perror("accept()"));
	user_socket.events = POLLIN | POLLOUT;
	user_socket.revents = 0;
	_sockets.push_back(user_socket);
	_users.insert(std::pair<int, User>( \
		user_socket.fd, User(user_socket.fd)));
	return (true);
}

/**
 * @brief 
 * Nicknames are non-empty strings with the following restrictions:
 * They MUST NOT contain any of the following characters: space (' ', 0x20), comma (',', 0x2C), asterisk ('*', 0x2A), question mark ('?', 0x3F), exclamation mark ('!', 0x21), at sign ('@', 0x40).
 * They MUST NOT start with any of the following characters: dollar ('$', 0x24), colon (':', 0x3A).
 * They MUST NOT start with a character listed as a channel type, channel membership prefix, or prefix listed in the IRCv3 multi-prefix Extension.
 * They SHOULD NOT contain any dot character ('.', 0x2E).
 * 
 * @param nick 
 * @return true 
 * @return false 
 */
bool	Server::isValidNick(const std::string &nick)
{
	int		fdbk;

	fdbk = true;
	if (nick.empty() || nick == BOT_NAME)
		return (false);
	fdbk = nick.find_first_of(" ,*?!@.#&$:") == std::string::npos && nick.find_first_of("#&$:") != 0;
	fdbk &= !std::isdigit(nick[0]);
	return (fdbk);
}

/**
 * @brief 
 * Nicknames must be unique in the server
 * 
 * @param nick 
 * @return true 
 * @return false 
 */
bool	Server::isUniqueNick(const std::string &nick)
{
	for (std::map<int, User>::iterator it = _users.begin(); \
			it != _users.end(); it++)
	{
		if ((*it).second.getNickname() == Libftpp::strToLower(nick))
			return (false);
	}
	return (true);
}

/**
 * @brief appends a message that is intended to the several users
 * 		at the end of their respective output buffers
 * 
 * @return true 
 * @return false 
 */
bool	Server::broadcasting(void)
{
	for (std::map<int, User>::iterator it = _users.begin(); \
			it != _users.end(); it++)
	{
		if ((*it).second.isRegistered())
		{
			(*it).second.insertOutMessage(broadcastMsg);
		}
	}
	broadcastMsg.clear();
	return true;
}

int				Server::removeChannel(const std::string& chann_name)
{
	channels.erase(Libftpp::strToLower(chann_name));
	return (true);
}

void			Server::updateChannels(void)
{
	for (std::map<std::string, Channel>::iterator it = channels.begin(); it != channels.end(); )
	{
		std::map<std::string, Channel>::iterator tmp = it++;
		if ((*tmp).second.getNbOfMembers())
			(*tmp).second.setNewOperator();
		else
			removeChannel((*tmp).first);
	}
	
}