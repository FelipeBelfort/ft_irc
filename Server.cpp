/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:14 by TheTerror         #+#    #+#             */
/*   Updated: 2024/04/20 16:58:28 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool				Server::is_connected = true;
uint16_t			Server::_port = 0;
std::string			Server::_password;
std::string			Server::broadcastMsg;
std::string			Server::sourcename;
std::vector<pollfd>	Server::_sockets;
std::map<int, User>	Server::_clients;

Server::Server()
{
}

Server::~Server()
{
}

/**
 * @brief 
 * It initiates the server. If the PORT is not a valid port or if socket() fails it returns false,
 *  if bind() or listen() fails it closes the fd and return false.
 * In case of success it pushs the server socket to the list of sockets and returns true. 
 * The password is setted by default
 * 
 * @param port expected a int between MIN_PORT and MAX_PORT
 * @param password 
 * @return boolean
 */
bool	Server::initServer(const std::string &port, const std::string &password)
{
	float		p;
	pollfd		server_socket;
	sockaddr_in	server_addr;


	if (!Libftpp::strIsInt(port)) 
		return (Libftpp::error("invalid port number '" + port + "'"));
	p = atof(port.c_str());
	if (p < MIN_PORT || p > MAX_PORT)
		return (Libftpp::error("invalid port number '" + port + "'" \
			+ "\nvalid range: " + Libftpp::itoa(MIN_PORT) + " < port " \
			+ " > " + Libftpp::itoa(MAX_PORT)));
	_port = p;
	_password = password;

	server_socket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket.fd == -1)
		return (Libftpp::ft_perror("socket()"));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket.fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
		return (close(server_socket.fd), Libftpp::ft_perror("bind()"));
	if (listen(server_socket.fd, MAX_CONNECTIONS))
		return (close(server_socket.fd), Libftpp::ft_perror("listen()"));
	server_socket.events = POLLIN;

	_sockets.push_back(server_socket);
	sourcename.assign((std::string) HOSTNAME + ":" + port);
	return (true);
}

/**
 * @brief 
 * It calls the initServer() and if it returns true 
 *  then it calls the signal() to catch 'ctrl+c' and launchs the loop with pool() to listen 
 * the sockets
 *  
 * TODO => error management and the actions inside the for loop
 * 
 * @param port  to be used in initServer() 
 * @param password  to be used in initServer()
 * @return boolean
 */
bool	Server::launchServer(const std::string &port, const std::string &password)
{
	int			poll_ret;

	poll_ret = -111;
	if (!initServer(port, password))
		return (false);
	signal(SIGINT, &exitServer);
	while (is_connected)
	{
		_sockets[0].revents = 0;
		poll_ret = poll(&_sockets[0], _sockets.size(), 10);
		if (poll_ret < 0)
			return (Libftpp::ft_perror("poll()"));
		else if (!poll_ret)
			continue; // timeout
		if (_sockets[0].revents & POLLIN)
			if (!createUser())
				return (false);
		if (!loopOnUsers())
			return (false);
	}
	std::cout << "\nExiting Server..." << std::endl; //TODO clean up all because it's static class
	return (true);
}

/**
 * @brief it loops on all sockets created
 * 		- checking eventual I/O errors flagged by poll,
 * 		- then eventually reading incoming data by the respective fd
 * 
 * @return true 
 * @return false 
 */
bool	Server::loopOnUsers()
{
	int			fdbk;
	char		buff[BUFF_SIZE];
	int			read_ret;

	fdbk = true;
	read_ret = -111;
	for (size_t i = 1; i < _sockets.size(); )
	{
		if (_sockets[i].revents & POLLERR) // TODO verify error cases before
			std::cout << "client => " << _sockets[i].fd << " POLLERR " << std::endl;
		if (_sockets[i].revents & POLLHUP)
			std::cout << "client => " << _sockets[i].fd << " POLLHUP " << std::endl;
		if (_sockets[i].revents & POLLERR || _sockets[i].revents & POLLHUP)
		{
			if (!closeClient(i))
				return (false);
			continue;
		}
		if (_sockets[i].revents & POLLIN)
		{
			memset(buff, 0, BUFF_SIZE);
			read_ret = recv(_sockets[i].fd, buff, BUFF_SIZE - 1, MSG_DONTWAIT);
			if (read_ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
			{
				i++;
				continue;
			}
			else if (read_ret < 0)
				return (Libftpp::ft_perror("recv()"));
			else if (!read_ret)
			{
std::cout << "client => " << _sockets[i].fd << " closed " << std::endl;
			}
			buff[read_ret] = 0;
			fdbk = _clients.at(_sockets[i].fd).routine(buff);
			if (fdbk == _fatal)
			{
				if (!closeClient(i))
					return (false);
std::cout << "user deleted"<< std::endl;
				continue;
			}
			else if (!fdbk)
				return (false);
		}
		if (_sockets[i].revents & POLLOUT)
		{
			fdbk = _clients.at(_sockets[i].fd).routine("");
			if (fdbk == _fatal)
			{
				if (!closeClient(i))
					return (false);
std::cout << "user deleted"<< std::endl;
				continue;
			}
			else if (!fdbk)
				return (false);
		}
		_sockets[i].revents = 0;
		i++;
	}
	return (true);
}

/**
 * @brief 
 * If the poll.revents faces a problem this fonction will be called to close the socket and erase both the User and the 
 * pollfd from the list. 
 * 
 * @param i the index of the revent in the pollfd list
 * @return boolean
 */
bool	Server::closeClient(int i)
{
	_clients.erase(_sockets[i].fd);
	if (close(_sockets[i].fd) < 0)
		return (Libftpp::ft_perror("close()"));
	_sockets.erase(_sockets.begin() + i);
	return (true);

}

/**
 * @brief 
 * Fonction to create a user socket and push it in the list
 * @return boolean
 * TODO => error management
 * 
 */
bool	Server::createUser(void)
{
	pollfd	user_socket;

	user_socket.fd = accept(_sockets[0].fd, NULL, NULL);
	if (user_socket.fd == -1)
		return (false); // TODO error msg
	user_socket.events = POLLIN | POLLOUT;
	_sockets.push_back(user_socket);
	_clients.insert(std::pair<int, User>(user_socket.fd, User(_sockets.back())));
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
	if (nick.empty())
		return (false);
	fdbk = nick.find_first_of(" ,*?!@.") == std::string::npos && nick.find_first_of("#$:") != 0;
	fdbk &= !std::isdigit(nick[0]);
	// TODO add rule They MUST NOT start with a character listed as a channel type, channel membership prefix, or prefix listed in the IRCv3 multi-prefix Extension.
	return (fdbk);
}

/**
 * @brief 
 * Nicknames must be unique in the server
 * 
 * 	TODO verify if is case insensitive and do the same for the channel
 * @param nick 
 * @return true 
 * @return false 
 */
bool	Server::isUniqueNick(const std::string &nick)
{
	for (std::map<int, User>::iterator it = _clients.begin(); \
			it != _clients.end(); it++)
	{
		if ((*it).second.getNickname() == Libftpp::strToLower(nick))
			return (false);
	}
	return (true);
}

bool	Server::broadcasting(void)
{
	for (std::map<int, User>::iterator it = _clients.begin(); \
			it != _clients.end(); it++)
	{
		if ((*it).second.isRegistered())
		{
			(*it).second.insertOutMessage(broadcastMsg);
		}
	}
	broadcastMsg.clear();
// std::cout << "**************************broadcasted!" << std::endl;
	return true;
}

/**
 * @brief 
 * Handle the SIGINT and changes the is_connected state to exit the server
 * 
 * @param sign 
 */
void	exitServer(int sign)
{
	if (sign == SIGINT)
		Server::is_connected = false;
}
