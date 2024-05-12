/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jm <jm@student.42lyon.fr>                  +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/11 20:26:14 by TheTerror         #+#    #+#             */
/*   Updated: 2024/05/12 14:17:06 by jm               ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/* *********************** 'Server' Class attributes ************************ */
bool				Server::is_connected = true;
bool				Server::isPionner_flag = true;
uint16_t			Server::_port = 0;
std::string			Server::_password;
std::string			Server::broadcastMsg;
std::string			Server::sourcename;
std::string			Server::_startDate;
std::vector<pollfd>	Server::_sockets;
std::map<int, User>	Server::_users;
std::map<std::string, Channel>	Server::channels;
/* ************************************************************************** */

Server::Server()
{
}

Server::~Server()
{
}

/**
 * @brief 
 * It calls the initServer() and if it returns true 
 *  then it calls the signal() to catch 'ctrl+c' and launchs the loop with poll() to listen 
 * the sockets
 *  
 * TODO => error management and the actions inside the for loop
 * 
 * @param port  the access port to the server
 * @param password  password connection the server
 * @return true, false
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
			is_connected = Libftpp::ft_perror("poll()");
		else if (!poll_ret) // timeout
			continue;
		if (is_connected && _sockets[0].revents & POLLIN)
			if (!createUser())
				is_connected = false;
		if (is_connected && !loopOnUsers())
			is_connected = false;
	}
	std::cout << "\nExiting Server..." << std::endl;
	if (!atExitProcess())
		return (false);
	return (true);
}

/**
 * @brief 
 * It initiates the server. If the PORT is not a valid port or if socket() fails it returns false,
 *  if bind() or listen() fails it closes the fd and return false.
 * In case of success it pushes the server socket to the list of sockets and returns true. 
 * The password is setted by default
 * 
 * @param port expected, a int between MIN_PORT and MAX_PORT
 * @param password 
 * @return true, false
 */
bool	Server::initServer(const std::string &port, const std::string &password)
{
	float		p;
	int			option;
	pollfd		server_socket;
	sockaddr_in	server_addr;
	time_t		tm;

	option = 1;
	if (!Libftpp::strIsInt(port)) 
		return (Libftpp::error("invalid port number '" + port + "'"));
	p = atof(port.c_str());
	if (p < MIN_PORT || p > MAX_PORT)
		return (Libftpp::error("invalid port number '" + port + "'" \
			+ "\nvalid range: " + Libftpp::itoa(MIN_PORT) + " < port > " \
			+ Libftpp::itoa(MAX_PORT)));
	_port = p;
	_password = password;
	server_socket.fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket.fd == -1)
		return (Libftpp::ft_perror("socket()"));
	if (setsockopt(server_socket.fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, \
	&option, sizeof(option)) < 0)
		return (Libftpp::ft_perror("setsockopt()"));
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(_port);
	server_addr.sin_addr.s_addr = INADDR_ANY;

	if (bind(server_socket.fd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
		return (close(server_socket.fd), Libftpp::ft_perror("bind()"));
	if (listen(server_socket.fd, MAX_CONNECTIONS))
		return (close(server_socket.fd), Libftpp::ft_perror("listen()"));
	server_socket.events = POLLIN;
	tm = std::time(NULL);
	_startDate.assign(std::asctime(std::localtime(&tm)));
	Libftpp::trim(_startDate, " \n");
	_sockets.push_back(server_socket);
	sourcename.assign((std::string) HOSTNAME + ":" + port);
	return (true);
}

/**
 * @brief it loops on all sockets created
 * 		- checking eventual I/O errors flagged by poll,
 * 		- then eventually reading incoming data by the respective fd
 * @return true 
 * @return false 
 */
bool	Server::loopOnUsers()
{
	int			fdbk;
	char		buff[BUFF_SIZE];
	int			recv_ret;

	fdbk = true;
	recv_ret = -111;
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
			recv_ret = recv(_sockets[i].fd, buff, BUFF_SIZE - 1, MSG_DONTWAIT);
			if (recv_ret < 0 && (errno == EAGAIN || errno == EWOULDBLOCK))
			{
				i++;
				continue;
			}
			else if (recv_ret < 0)
				return (Libftpp::ft_perror("recv()"));
			else if (!recv_ret)
			{
/*DEBUG*/std::cout << "client => " << _sockets[i].fd << " recv_ret = 0" << std::endl;
				if (!closeClient(i))
					return (false);
				continue;
			}
			buff[recv_ret] = 0;
			fdbk = _users.at(_sockets[i].fd).routine(i, buff);
			if (fdbk == _fatal)
			{
				if (!closeClient(i))
					return (false);
/*DEBUG*/std::cout << "user deleted"<< std::endl;
				continue;
			}
			else if (!fdbk)
				return (false);
// std::cerr << ":::::::::::: debugging ::::::::::::: " << '\n';
		}
		if (_sockets[i].revents & POLLOUT)
		{
			fdbk = _users.at(_sockets[i].fd).routine(i, "");
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
 * Handle the SIGINT by toggling the 'is_connected' flag to exit the server
 * 
 * @param sign 
 */
void	exitServer(int sign)
{
	if (sign == SIGINT)
		Server::is_connected = false;
}

int		Server::atExitProcess(void)
{
	_users.clear();
	channels.clear();
	for (size_t i = _sockets.size(); i > 0; i = _sockets.size())
		closeClient(i - 1);
	_sockets.clear();
	return (true);
}
